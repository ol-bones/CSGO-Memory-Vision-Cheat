#pragma once
#include "stdafx.h"

using namespace std;

struct EntityBuffer
{
	int BufferIndex;
	DWORD Address1;
};

class Cheat
{
public:
	Cheat();
	int CheatEntry(int argc, char* argv[]);

	void Update();

private:
	DWORD LocalPlayerAddress;
	DWORD EntitiesAddress;
	DWORD DWLocalPlayer;
	vector<Player> Players;
	LocalPlayer lPlayer;

	UINT_PTR MemLocs[15];
	vector<EntityBuffer> EntityBuffers;
	DWORD buffer[64];
	SIZE_T bufferSize;
};