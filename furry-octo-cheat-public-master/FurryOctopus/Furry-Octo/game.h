#pragma once
#include "stdafx.h"
class Game
{
public:
	Game() { };
	~Game() { };
	static HMODULE CLIENT;
	static HMODULE ENGINE;

	static HWND hWindow;
	static DWORD ProcessID;
	static HANDLE hProcess;
};