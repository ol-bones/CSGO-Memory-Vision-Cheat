#include "stdafx.h"
#include "game.h"
#include "vector3.h"
#include "LocalPlayer.h"

using namespace std;

LocalPlayer::LocalPlayer(int r) : Player(r)
{
	this->m_TriggerToggle = false;
	SystemParametersInfo(SPI_GETMOUSESPEED, 0, this->PTR_MouseSpeed, 0);
	this->OriginalMouseSpeed = this->MouseSpeed;
};

LocalPlayer::LocalPlayer() { };
LocalPlayer::~LocalPlayer() { };

#pragma region Unused Mouse Code
// this mouse shit doesnt work.
// msdn and stackoverflow solutions dont work.
int OriginalMouseSpeed = 0;
int MouseSpeed = 0, *PTR_MouseSpeed = &MouseSpeed;
bool Burst = true;

void LocalPlayer::ResetMouseSpeed()
{
	SystemParametersInfo(SPI_SETMOUSESPEED,
		0,
		(LPVOID)OriginalMouseSpeed,
		SPIF_UPDATEINIFILE ||
		SPIF_SENDCHANGE ||
		SPIF_SENDWININICHANGE);
}

void LocalPlayer::SetMouseSpeed(int s)
{
	SystemParametersInfo(SPI_SETMOUSESPEED,
		0,
		(LPVOID)s,
		SPIF_UPDATEINIFILE ||
		SPIF_SENDCHANGE ||
		SPIF_SENDWININICHANGE);
}
#pragma endregion

bool LocalPlayer::IsKeyDown(int Key)
{
	SHORT wKeyState = GetAsyncKeyState(Key);

	// Test high bit - if set, key was down when GetAsyncKeyState was called.
	if ((1 << 16) & wKeyState)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void LocalPlayer::LeftClickBurst()
{
	INPUT    Input = { 0 };

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));

	ZeroMemory(&Input, sizeof(INPUT));
	int r = rand() % 25;
	Sleep(160 + r);

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void LocalPlayer::LeftClick()
{
	INPUT    Input = { 0 };

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));

	ZeroMemory(&Input, sizeof(INPUT));
	int r = rand() % 5;
	Sleep(15 + r);

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void LocalPlayer::Update(int debug)
{
	this->UpdateID();
	this->UpdateTeam();
	this->UpdateHealth();
	this->UpdatePosition();
	this->UpdateDormant();

	if (debug == 1)
	{
		return;
	}
	if (debug == 2)
	{
		cout << a1 << a2 << a3 << a4 << a5 << a6 << endl;
	}

	cout << "PID:  " << this->GetID() << endl;
	cout << "Local: " << this->IsLocalPlayer() << endl;
	cout << "HP:   " << this->GetHealth() << endl;
	cout << "Team: " << this->GetTeam() << endl;
	cout << "Pos:  " << this->GetPos() << endl;
	cout << "Dorm: " << this->GetDormant() << endl;
	cout << "AimID:" << this->GetAimEntityID() << endl;
	cout << endl;
}

void LocalPlayer::SetMemoryLocations(UINT_PTR cida[])
{
	for (int i = 0; i < 15; i++)
	{
		this->Addresses[i] = cida[i];
	}
}

const int& LocalPlayer::GetID() const { return m_ID; };
const int& LocalPlayer::GetTeam() const { return m_Team; };
const int& LocalPlayer::GetHealth() const { return m_Health; };
const Vector3& LocalPlayer::GetPos() const { return m_Position; };
const bool& LocalPlayer::IsLocalPlayer() const { return m_isLocal; }
const int& LocalPlayer::GetDormant() const { return m_bDormant; }

void LocalPlayer::SetID(int i) { this->m_ID = i; };
void LocalPlayer::SetTeam(int t) { this->m_Team = t; };
void LocalPlayer::SetHealth(int h) { this->m_Health = h; };
void LocalPlayer::SetPos(Vector3 p) { this->m_Position = p; };

int LocalPlayer::GetAimEntityID()
{
	int xID;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[0]), &xID, sizeof(xID), 0);
	return xID;
}

void LocalPlayer::UpdateDormant()
{
	int intDormant;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[4]), &intDormant, sizeof(intDormant), 0);
	//this->SetDormant(intDormant);
}

void LocalPlayer::UpdateTeam()
{
	int intTeam;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[2]), &intTeam, sizeof(intTeam), 0);
	this->SetTeam(intTeam);
}

void LocalPlayer::UpdateID()
{
	int intID;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[5]), &intID, sizeof(intID), 0);
	this->SetID(intID);
}

void LocalPlayer::UpdateHealth()
{
	int intHealth;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[3]), &intHealth, sizeof(intHealth), 0);
	this->SetHealth(intHealth);
}

void LocalPlayer::UpdatePosition()
{
	float vecPos[3];
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[1]), &vecPos, sizeof(vecPos), 0);
	this->SetPos(Vector3(vecPos[0], vecPos[1], vecPos[2]));
}