#include "stdafx.h"
#include "game.h"
#include "player.h"
#include "LocalPlayer.h"
#include "vector2.h"
#include "vector3.h"
#include "scatter_string.h"
#include <ctime>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "user32.lib")

using namespace std;

// THIS IS THE UNIQUE ID FOR THIS EXE
const string ProgramID = "0000001";

HMODULE Game::CLIENT;
HMODULE Game::ENGINE;
HWND Game::hWindow;
DWORD Game::ProcessID;
HANDLE Game::hProcess;

int CSModule(TCHAR path[])
{
	string spath(path);
	string modname;

	for (int i = spath.length(); i >= 0; i--)
	{
		if (spath[i] == '\\')
		{
			modname = spath.substr(i + 1, spath.length());
			break;
		}
	}
	//cout << "Name:" << modname << endl;

	if (modname == ("client.dll")) { return 1; }
	if (modname == ("engine.dll")) { return 2; }
	return 0;
}

#pragma region Decoy

class Place
{
public:
	string leftBrace;
	string middle;
	string rightBrace;

	Place()
	{
		leftBrace = "[";
		middle = " ";
		rightBrace = "]";
	}

	void PrintPlace()
	{
		if (middle == " ")
		{
			cout << leftBrace << middle << rightBrace;
		}
		else
		{
			cout << leftBrace;
			cout << middle;
			cout << rightBrace;
		}
	}
};

class Row
{
public:
	Place * Places[10];

	Row()
	{
		for (int i = 0; i<10; i++)
		{
			Places[i] = new Place();
		}
	}

	void PrintRow()
	{
		for (int i = 0; i<10; i++)
		{
			Places[i]->PrintPlace();
		}
		cout << endl;
	}
};

class Board
{
public:
	Row * Rows[10];

	Board()
	{
		for (int i = 0; i<10; i++)
		{
			Rows[i] = new Row();
		}
	}

	Place *GetSpawn()
	{
		return Rows[9]->Places[5];
	}

	Place *GetPlace(const Vector2* p)
	{
		return Rows[p->y]->Places[p->x];
	}

	void PrintBoard()
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		for (int i = 0; i < csbi.dwSize.Y / 2; i++)
		{
			cout << endl;
		}
		for (int i = 0; i<10; i++)
		{
			Rows[i]->PrintRow();
		}
		cout << "--------- Keys ---------\n[  \\  ]   - Hold to fire\n[ INS ]   - Refresh mem \n[ HOME]   - Toggle Burst/Single Fire\n[ F12 ]   - Close gracefully" << endl;
	}
};

class Spaceship
{
public:
	Vector2 * Position;
	Place * curPlace; // want to define pointer to a place object
	Board * cboard;

	Spaceship(Board *b) // want b to be pointer to board1
	{
		cboard = b;
		Position = new Vector2(5, 9);
		curPlace = b->GetSpawn(); // then run getspawn, which should return a Place object
		curPlace->middle = "x"; // this doesn't change x, and it's also making error "must have class/struct/union"
	}

	void Move(const Vector2* vec)
	{
		if (Position->x + vec->x <= 9 && Position->x + vec->x >= 0
			&& Position->y + vec->y <= 9 && Position->y + vec->y >= 0)
		{
			curPlace->middle = " ";
			Position->x += vec->x;
			Position->y += vec->y;
			curPlace = cboard->GetPlace(Position);
			curPlace->middle = "x";
			cboard->PrintBoard();
			cout << "Position: " << Position->x << "," << Position->y << endl;
		}
	}
};

double& clamp(double& value, double& low, double& high)
{
	if (value > high)
	{
		return high;
	}
	else if (value < low)
	{
		return low;
	}
	else
	{
		return value;
	}	
}

#pragma endregion

char* shitstringtonormalstring(LPSTR& shit)
{
	char* shite;
	shite = (char*)shit;
	return shite;
}

char* shittonormalagain(char* szU8)
{
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	wszString[wcsLen] = '\0';

	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansiLen + 1];
	::WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
	szAnsi[ansiLen] = '\0';

	delete[]wszString;
	return szAnsi;
}

inline UINT_PTR String_To_UINT_PTR(const std::string& s)
{
	UINT_PTR intptr = 0x000000;
	intptr = stoul(s, nullptr, 0);
	if (intptr == 0 || intptr == 0x000000)
	{
		throw new exception("Error: UINT_PTR to c_string failed (String_To_UINT_PTR)");
	}
	return intptr;
}

int main(int argc, char* argv[])
{

	// Initialize the rand() function using the current time as the seed
	srand(time(NULL));

#ifdef _DEBUG

	cout << "Debug Mode" << endl;

	// CLAMP TEST
	/*double a = 0.01020310000;
	double b = 0.03034943424;
	double c = 0.5;

	double result = clamp(b, a, c);
	cout << result << endl;*/

	// SCATTER STRING TEST
	/*ScatterString wtitle("Counter-Strike: Global Offensive");
	string x = wtitle.Retrieve();

	ScatterString muffins("This shit be getting hidden brah...");
	x = muffins.Retrieve();*/
	
	cout << "Command-line arguments:" << endl;
	for (int i = 1; i < argc; i++)
	{
		cout << "  argv[" << i << "]   " << argv[i] << endl;
	}

#else
	// Anti-Debug Code
	__asm
	{
		push ss
		lel : pop ss
			  jmp mad
			  jmp lel
		  mad : pop eax
	}
	DWORD OldProtect = 0;

	char *pBaseAddr = (char*)GetModuleHandle(NULL);

	VirtualProtect(pBaseAddr, 4096,
		PAGE_READWRITE, &OldProtect);

	ZeroMemory(pBaseAddr, 4096);

	HANDLE hThread = GetCurrentThread();
	typedef NTSTATUS(NTAPI *pNtSetInformationThread)
		(HANDLE, UINT, PVOID, ULONG);
	NTSTATUS Status;

	string f = "dsfsfds";
	f += 324324;
	string* ff = &f;
	ff += 32424;
	ff -= 242;

	pNtSetInformationThread NtSIT = (pNtSetInformationThread)
		GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")),
		"NtSetInformationThread");

	double b = rand() % 213123;
	int a = int(new float(b));
	double* d = &b;
	d += 10;
	if (NtSIT == NULL)
	{
		_asm
		{
			push ss
			lela : pop ss
				   jmp mada
				   jmp lela
			   mada : pop eax
					  XOR AX, AX
		}
		return 0;
	}
	Board* junkBoard = new Board();
	if (hThread == NULL)
	{

		Status = NtSIT(GetCurrentThread(),
			0x11,
			0, 0);
	}
	else
	{
		Status = NtSIT(hThread, 0x11, 0, 0);
	}
#pragma endregion
#endif

	// Argument argv[0] is the executable path
	string ranmem = "";
	ranmem = argv[1];
	string s_uid = argv[2];

	//cout << ranmem << endl;
	const int shit1 = atoi(ranmem.c_str());
	std::vector<DWORD> memshit(shit1);
	std::vector<float> memshit1(shit1 / 2);
	for (int i = 0; i < shit1 / 2; i++)
	{
		memshit1[i] = float(rand() % 423432);
	}						

#pragma region shite
	//cout << s_uid << " && " << ProgramID << endl;
	//cout << sizeof(s_uid) << " && " << sizeof(ProgramID) << endl;
#pragma endregion

	if (s_uid != ProgramID) // if this is true then they tried launching the app directly or the id is wrong 
	{
		cout << "Unhandled Run-Time Exception 890 (Insufficient Priviledges): \n - Ws2_32 0x42524 null" << endl; // le fake error
		Sleep(500); // give them time to see the error i guess
		_asm
		{
		wut: XOR AX, AX // break stuff
			jmp wut// break stuff
		kek : pop ss // infinite loop
		  heh : jmp kek
				jmp heh // infinite loop
				jmp wut // even more loop

		} // attempt to break stuff again before it closes
		return 0; // just close the program if it isnt broken already
	}


	__asm
	{
		mov eax, fs:[0x30] // PEB
			PUSH DS
			POP DS
			PUSH DS
			POP DS
			PUSH DS
			POP DS
			PUSH DS
			POP DS
			cmp al, 0
			mov al, 0
			push al
			mov eax, [eax + 0x0c] // PEB_LDR_DATA
			mov eax, [eax + 0x0c] // InOrderModuleList
			mov dword ptr[eax + 0x20], 0x999999 // SizeOfImage
	}
	HWND cli = GetConsoleWindow();
	SetWindowPos(cli, 0, 100, 600, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	Board * board1 = new Board(); // creates new board, which makes 10 rows with 10 places

	Spaceship * spaceship1 = new Spaceship(board1); // want to pass ref of board1

	cout << ":: Launch successful ::" << endl;

	HMODULE CLIENT;
	HMODULE ENGINE;

	HMODULE hMods[1024];
	HWND hWindow;
	std::vector<float> memshit2(shit1 * 2);
	for (int i = 0; i < shit1 * 2; i++)
	{
		memshit2[i] = float(rand() % 6423432);
	}
	DWORD processID;
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;
	
	hWindow = FindWindow(NULL, "Counter-Strike: Global Offensive");
	if (!hWindow || hWindow == NULL)
	{
		cout << ":: Looking for CSGO ::" << endl;
	}
	while (!hWindow || hWindow == NULL)
	{
		hWindow = FindWindow(NULL, "Counter-Strike: Global Offensive");
		Sleep(100);
	}
	cout << "hWindow: " << hWindow << endl;


	GetWindowThreadProcessId(hWindow, &processID);
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	while (!hProcess || hProcess == NULL)
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
		Sleep(100);
	}
	cout << "hProcess: " << hProcess << endl;
	cout << "processID: " << processID << endl;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				// Print the module name and handle value.

				switch (CSModule(szModName))
				{
				case 1:
					CLIENT = hMods[i];
					_tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
					break;
				case 2:
					ENGINE = hMods[i];
					_tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
					break;
				}
			}
		}
	}

	if (CLIENT == NULL)
	{
		cout << "client.dll not found" << endl;
		return 0;
	}
	if (ENGINE == NULL)
	{
		cout << "engine.dll not found" << endl;
		return 0;
	}
	

	Game::ProcessID = processID;
	Game::hProcess = hProcess;
	Game::hWindow = hWindow;
	Game::CLIENT = CLIENT;
	Game::ENGINE = ENGINE;

	DWORD LocalPlayerAddress = (UINT_PTR)CLIENT + String_To_UINT_PTR(argv[4]); // client + (uint)(string)localplayeroffset
	DWORD EntitiesAddress = ((INT_PTR)CLIENT) + String_To_UINT_PTR(argv[3]);
	cout << EntitiesAddress << endl;
	DWORD DWLocalPlayer;
	vector<Player> Players = vector<Player>();
	LocalPlayer lPlayer = LocalPlayer(shit1);

	UINT_PTR MemLocs[15];
	for (int i = 5; i < argc; i++)
	{
		MemLocs[i - 5] = String_To_UINT_PTR(argv[i]);
	}

	lPlayer.SetMemoryLocations(MemLocs);

	struct EntityBuffer
	{
		int BufferIndex;
		DWORD Address1;
	};

	vector<EntityBuffer> EntityBuffers;
	DWORD buffer[64];
	SIZE_T bufferSize;

	int on = 0;

	while (true)
	{
		if (lPlayer.IsKeyDown(0x7B)) // F12
		{
			// Exit 
			board1->PrintBoard();

			HMODULE CLIENT = NULL;
			HMODULE ENGINE = NULL;

			HWND hWindow = NULL;
			DWORD processID = NULL;
			HANDLE hProcess = NULL;
			DWORD cbNeeded = NULL;
			CloseHandle(Game::hProcess);

			board1->PrintBoard();
			return 0;
		}
		if (lPlayer.IsKeyDown(0x2D)) // Insert
		{
			ReadProcessMemory(hProcess, (void*)LocalPlayerAddress, &DWLocalPlayer, sizeof(DWLocalPlayer), 0);

			EntityBuffers.clear();
			DWORD buffer[64];
			SIZE_T bufferSize;

			ReadProcessMemory(hProcess, (void*)EntitiesAddress, &buffer, (sizeof(DWORD)* (64)), &bufferSize);
			for (int i = 0; i < 64; i++)
			{
				EntityBuffer eb = EntityBuffer();
				eb.Address1 = buffer[i];
				eb.BufferIndex = i;
				EntityBuffers.push_back(eb);
			}
			Players.clear();

			lPlayer.SetAddress(EntityBuffers[0].Address1);
			lPlayer.Update(1);

			cout << "-------------[ LOCAL PLAYER ]-------------" << endl;

			cout << "Player: " << lPlayer.GetID() << endl;
			cout << " > Position: " << lPlayer.GetPos() << endl;
			cout << " > Health: " << lPlayer.GetHealth() << endl;
			cout << " Team: " << lPlayer.GetTeam() << endl;
			cout << "XHair: " << lPlayer.GetAimEntityID() << endl;

			cout << "-----------------------------------------" << endl;

			for (int i = 0; i < 64; i += 4)
			{
				Player p = Player(shit1);

				p.SetAddress(EntityBuffers[i].Address1);
				p.SetMemoryLocations(MemLocs);
				p.Update(1);

				if (p.GetID() <= 64 && p.GetID() >= 2)
				{
					Players.push_back(p);
				}
			}
			cout << "-------------[ PLAYER LIST ]-------------" << endl;
			for (int i = 0; i < Players.size(); i++)
			{
				cout << "Player: " << Players[i].GetID() << endl;
				cout << " > Position: " << Players[i].GetPos() << endl;
				cout << " > Health: " << Players[i].GetHealth() << endl;
				cout << " Team: " << Players[i].GetTeam() << endl;
				cout << "XHair: " << Players[i].GetAimEntityID() << endl;
			}
			cout << "-----------------------------------------" << endl;
			Sleep(120);
		}
		if (on == 1)
		{
			//board1->PrintBoard();
			if (lPlayer.IsKeyDown(0x57))
			{
				spaceship1->Move(&Vector2(0, 1));
			}

			if (lPlayer.IsKeyDown(0x41))
			{
				spaceship1->Move(&Vector2(-1, 0));
			}

			if (lPlayer.IsKeyDown(0x53))
			{
				spaceship1->Move(&Vector2(0, -1));
			}

			if (lPlayer.IsKeyDown(0x44))
			{
				spaceship1->Move(&Vector2(1, 0));
			}

			on = 0;
		}
		else
		{
			on = 1;
		}
		if (lPlayer.IsKeyDown(0x24)) // Home
		{
			lPlayer.Burst = !lPlayer.Burst;
			Sleep(120);
		}
		if (lPlayer.GetID() <= -1)
		{
			continue;
		}
		if (lPlayer.IsKeyDown(0x23)) // End
		{
			lPlayer.m_TriggerToggle = !lPlayer.m_TriggerToggle;
			Sleep(300);
		}
		if (lPlayer.m_TriggerToggle == true || lPlayer.IsKeyDown(0x5A)) // z
		{
			lPlayer.Update(0);

			int curEntity = lPlayer.GetAimEntityID();
			cout << curEntity << endl;
			if (curEntity != 0 && curEntity != -1)
			{
				for (int i = 0; i < Players.size(); i++)
				{
					if (curEntity == Players[i].GetID())
					{
						if (Players[i].GetTeam() != lPlayer.GetTeam())
						{
							if (lPlayer.Burst)
							{
								lPlayer.LeftClickBurst();
								Sleep(20);
							}
							else
							{
								lPlayer.LeftClick();
								Sleep(100);
							}
						}
					}
				}
			}
		}
		Sleep(12);
	}

	// Release the handle to the process.
	CloseHandle(hProcess);

	return 0;
}

