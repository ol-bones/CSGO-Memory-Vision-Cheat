// Furry-Octo.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "app.h"
#include <objidl.h>
#include <gdiplus.h>
#include "game.h"
#include "player.h"
#include "localplayer.h"
#include "vector3.h"
#include "scatter_string.h"
#include <ctime>
#include "decrypt_string.h"

#pragma comment (lib,"Gdiplus.lib")

#define MAX_LOADSTRING 100

using namespace Gdiplus;
using namespace std;

// UNIQUE ID
const string ProgramID = "0000001";

const int MaxCommandLength = 100;
const bool DrawWindow = true;

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of s included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int, LPTSTR);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Game variables
HMODULE Game::CLIENT;
HMODULE Game::ENGINE;
HWND Game::hWindow;
DWORD Game::ProcessID;
HANDLE Game::hProcess;

HDC          hdcMem;
HBITMAP      hbmMem;
HANDLE       hOld;

PAINTSTRUCT  ps;

Cheat m_Cheat;

HWND m_hWnd;
HDC m_hdc;
HDC m_BackBuffer;
Console m_Console;
ESP m_ESP;
Vector2 m_DesktopResolution;

#define _WIN32_WINNT 0x050

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;

	if (nCode == HC_ACTION)
	{
		bool eat = m_Console.isOpen(); // if the console is open the user is typing so eat keystrokes om nom nom
		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			if (fEatKeystroke = (p->vkCode))
			{
				if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)) // Keydown
				{
					switch (p->vkCode)
					{
					case 0x70: eat = true; m_Console.ToggleOpen(); break; // F1
					case 0x08: m_Console.Backspace(); break;
					case 0x0D: m_Console.Enter(); break;
					case 0x20: m_Console.KeyPress((wchar_t*)L" \0"); break;
					case 0x30: m_Console.KeyPress((wchar_t*)L"0\0"); break;
					case 0x31: m_Console.KeyPress((wchar_t*)L"1\0"); break;
					case 0x32: m_Console.KeyPress((wchar_t*)L"2\0"); break;
					case 0x33: m_Console.KeyPress((wchar_t*)L"3\0"); break;
					case 0x34: m_Console.KeyPress((wchar_t*)L"4\0"); break;
					case 0x35: m_Console.KeyPress((wchar_t*)L"5\0"); break;
					case 0x36: m_Console.KeyPress((wchar_t*)L"6\0"); break;
					case 0x37: m_Console.KeyPress((wchar_t*)L"7\0"); break;
					case 0x38: m_Console.KeyPress((wchar_t*)L"8\0"); break;
					case 0x39: m_Console.KeyPress((wchar_t*)L"9\0"); break;
					case 0x41: m_Console.KeyPress((wchar_t*)L"A\0"); break;
					case 0x42: m_Console.KeyPress((wchar_t*)L"B\0"); break;
					case 0x43: m_Console.KeyPress((wchar_t*)L"C\0"); break;
					case 0x44: m_Console.KeyPress((wchar_t*)L"D\0"); break;
					case 0x45: m_Console.KeyPress((wchar_t*)L"E\0"); break;
					case 0x46: m_Console.KeyPress((wchar_t*)L"F\0"); break;
					case 0x47: m_Console.KeyPress((wchar_t*)L"G\0"); break;
					case 0x48: m_Console.KeyPress((wchar_t*)L"H\0"); break;
					case 0x49: m_Console.KeyPress((wchar_t*)L"I\0"); break;
					case 0x4A: m_Console.KeyPress((wchar_t*)L"J\0"); break;
					case 0x4B: m_Console.KeyPress((wchar_t*)L"K\0"); break;
					case 0x4C: m_Console.KeyPress((wchar_t*)L"L\0"); break;
					case 0x4D: m_Console.KeyPress((wchar_t*)L"M\0"); break;
					case 0x4E: m_Console.KeyPress((wchar_t*)L"N\0"); break;
					case 0x4F: m_Console.KeyPress((wchar_t*)L"O\0"); break;
					case 0x50: m_Console.KeyPress((wchar_t*)L"P\0"); break;
					case 0x51: m_Console.KeyPress((wchar_t*)L"Q\0"); break;
					case 0x52: m_Console.KeyPress((wchar_t*)L"R\0"); break;
					case 0x53: m_Console.KeyPress((wchar_t*)L"S\0"); break;
					case 0x54: m_Console.KeyPress((wchar_t*)L"T\0"); break;
					case 0x55: m_Console.KeyPress((wchar_t*)L"U\0"); break;
					case 0x56: m_Console.KeyPress((wchar_t*)L"V\0"); break;
					case 0x57: m_Console.KeyPress((wchar_t*)L"W\0"); break;
					case 0x58: m_Console.KeyPress((wchar_t*)L"X\0"); break;
					case 0x59: m_Console.KeyPress((wchar_t*)L"Y\0"); break;
					case 0x5A: m_Console.KeyPress((wchar_t*)L"Z\0"); break;
					}
				}
				else if ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP)) // Keyup
				{
				}
				break;
			}
			break;
		}
		if (eat) { return(fEatKeystroke ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam)); }
		else { return(CallNextHookEx(NULL, nCode, wParam, lParam)); }
	}
}

Vector2 GetDesktopResolution()
{
	RECT desktop;

	GetWindowRect(GetDesktopWindow(), &desktop);

	int horizontal = desktop.right;
	int vertical = desktop.bottom;
	return Vector2(horizontal, vertical);
}

VOID OnPaint(HDC hdc)
{
	Graphics * graphics;
	graphics = new Graphics(hdc);
	Pen * pen = new Pen(Color(255, 0, 0, 255));

	graphics->DrawLine(pen, 0, 0, 200, 100);
	m_ESP.Draw(graphics, m_DesktopResolution);
	m_Console.Draw(graphics, m_DesktopResolution);
	/*
	for (int i = 0; i < 1000; i++)
	{
		int x = rand() % 300;
		int y = rand() % 300;
		SetPixel(hdc, x, y, RGB(rand() % 255, 0, 0));
	}
	*/
	graphics->Flush();
	delete(pen);
	delete(graphics);
}

// Application start point!

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	MSG msg;
	HACCEL hAccelTable;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FURRYOCTO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow, lpCmdLine))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FURRYOCTO));
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
	
	// Main message loop:
	bool Exit = false;
	while (!Exit)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				Exit = true;
			}
			if (msg.message != 420)
			{
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	UnhookWindowsHookEx(hhkLowLevelKybd);

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_FURRYOCTO));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_FURRYOCTO);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, LPTSTR lpCmdLine)
{
	HWND hWnd = NULL;

	hInst = hInstance; // Store instance handle in our global variable

	if (DrawWindow)
	{
		Vector2 ScreenSize = GetDesktopResolution();
		m_DesktopResolution = ScreenSize;

		hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_EX_TOPMOST,
			0, 0, ScreenSize.x, ScreenSize.y, NULL, NULL, hInstance, NULL);
		m_hWnd = hWnd;
		SetWindowLong(hWnd, GWL_STYLE, 0); // Remove border and min/max buttons
		SetMenu(hWnd, NULL); // Remove menu bar ( File, About etc )

		SetLayeredWindowAttributes(hWnd, 0, 50, LWA_ALPHA);
		SetLayeredWindowAttributes(hWnd,
			RGB(255, 255, 255),  // colorkey
			255,            // 0=transparent, 255=completely solid
			LWA_COLORKEY);
		SetWindowPos(hWnd,       // handle to window
			HWND_TOPMOST,  // placement-order handle
			0,     // horizontal position
			0,      // vertical position
			ScreenSize.x,  // width
			ScreenSize.y, // height
			SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE// window-positioning options
			);

		if (!hWnd)
		{
			return FALSE;
		}
	}

	m_Cheat = Cheat();

	m_Console = Console();
	m_Console.AddLine(lpCmdLine);

	int argc = __argc; // Argument count
	wchar_t** wargv = __wargv; // Argument table (wide)
	char** argv; // Argument table
	size_t wlen;
	size_t len;	

#ifdef _DEBUG
	// Print arguments
	for (int i = 0; i < argc; i++)
	{
		OutputDebugString(L"\n");
		OutputDebugString(wargv[i]);
	}
#endif

	argv = (char**) malloc(argc*sizeof(char*)); // Allocate memory of argument count
	for (int i = 0; i < argc; i++)
	{
		wlen = lstrlenW(wargv[i]); // Get length of wide character argument
		len = wcstombs(NULL, wargv[i], wlen); // Convert to length of regular argument
		argv[i] = (char*) malloc(len + 1); // Allocate memory of argument length (including null byte at end)
		wcstombs(argv[i], wargv[i], len + 1); // Convert
	}

	m_Cheat.CheatEntry(argc, argv);

	m_ESP = ESP();

	if (DrawWindow)
	{
		ShowWindow(hWnd, 3); // 3 = maximised window
		UpdateWindow(hWnd);
	}	

	return TRUE;
}
RECT rect; // Struct for the window width and height
HDC hdc; // Device context
HDC hdcBackBuffer; // Back buffer device context
HBRUSH hRectangleBrush = CreateSolidBrush(RGB(0, 0, 0)); // Rectangle brush
int width, height; // Window width and height
HBITMAP hBitmap; // Bitmap
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		// Get the width and height of the window
		GetClientRect(hWnd, &rect);
		width = rect.right;
		height = rect.bottom;

		// Create the back buffer
		hdcBackBuffer = CreateCompatibleDC(NULL);

		// Get the device context
		hdc = GetDC(hWnd);

		// Create a bitmap
		hBitmap = CreateCompatibleBitmap(hdc, width, height);

		// Select the bitmap
		SelectObject(hdcBackBuffer, hBitmap);

		ReleaseDC(hWnd, hdc);
		break; 
	case WM_ERASEBKGND:
			return 1;
	case WM_PAINT:

		BitBlt(hdcBackBuffer, 0, 0, width, height, NULL, NULL, NULL, WHITENESS);

		OnPaint(hdcBackBuffer);

		hdc = BeginPaint(hWnd, &ps);
		// Display the back buffer
		BitBlt(hdc, 0, 0, width, height, hdcBackBuffer, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);

		// Repaint the screen
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}