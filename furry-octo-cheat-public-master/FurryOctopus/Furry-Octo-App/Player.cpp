#include "stdafx.h"
#include "game.h"
#include "player.h"
#include "vector2.h"
#include "vector3.h"

using namespace std;

Player::Player(int r)
{
	RandomCompileTimeInt = r;
	std::vector<float> memshit1(RandomCompileTimeInt / 2);

	for (int i = 0; i < RandomCompileTimeInt / 2; i++)
	{
		memshit1[i] = (float)(rand() % 423432);
	}

	memshit1 = memshit1;
};
Player::Player() { };
Player::~Player() { };

int RandomCompileTimeInt;
std::vector<float> memshit1;



const int& Player::GetID() const { return m_ID; };
const int& Player::GetTeam() const { return m_Team; };
const int& Player::GetHealth() const { return m_Health; };
const Vector3& Player::GetPos() const { return m_Position; };
const bool& Player::IsLocalPlayer() const { return m_isLocal; }
const int& Player::GetDormant() const { return m_bDormant; }

const DWORD& Player::GetAddress() const { return m_Address; };
void Player::SetAddress(DWORD a) { m_Address = a; };
void Player::SetIsLocal(bool b) { m_isLocal = b; }

void Player::Update(int debug)
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

int Player::GetAimEntityID()
{
	int xID;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + 0x00002410), &xID, sizeof(xID), 0);
	return xID;
}
void Player::SetMemoryLocations(UINT_PTR cida[])
{
	for (int i = 0; i < 15; i++)
	{
		this->Addresses[i] = cida[i];
	}
}


string a1 = "SDF sdfg sdfg sdfg dsfFOSFS";
string a2 = "SDFSFgsdg dsg dsfg dsgfOSFS";
string a3 = "SDg sdfg sdfg dsg sdFSFOSFS";
Vector3 m_Position;
DWORD m_Address;
string a4 = "SDFSFg sdfg sdfg OSFS";
int m_Team;
string a5 = "SDFSsdfg sdfg sdfg sdfFOSFS";
int m_Health;
string a6 = "SDFSfg sdfg sdfg sdfg FOSFS";
int m_ID;
bool m_isLocal;
int m_bDormant;
UINT_PTR CrosshairIDAddress;

void Player::SetID(int i) { this->m_ID = i; };
void Player::SetTeam(int t) { this->m_Team = t; };
void Player::SetHealth(int h) { this->m_Health = h; };
void Player::SetPos(Vector3 p) { this->m_Position = p; };


void Player::UpdateDormant()
{
	int intDormant;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[4]), &intDormant, sizeof(intDormant), 0);
	//this->SetDormant(intDormant);
}

void Player::UpdateTeam()
{
	int intTeam;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[2]), &intTeam, sizeof(intTeam), 0);
	this->SetTeam(intTeam);
}

void Player::UpdateID()
{
	int intID;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[5]), &intID, sizeof(intID), 0);
	this->SetID(intID);
}

void Player::UpdateHealth()
{
	int intHealth;
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[3]), &intHealth, sizeof(intHealth), 0);
	this->SetHealth(intHealth);
}

void Player::UpdatePosition()
{
	float vecPos[3];
	ReadProcessMemory(Game::hProcess, (void*)(GetAddress() + Addresses[1]), &vecPos, sizeof(vecPos), 0);
	this->SetPos(Vector3(vecPos[0], vecPos[1], vecPos[2]));
}