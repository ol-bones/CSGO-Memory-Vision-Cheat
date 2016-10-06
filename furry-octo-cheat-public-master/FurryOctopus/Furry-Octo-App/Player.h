#ifndef player_h
#define player_h

#include "stdafx.h"
#include "vector3.h"


class Player
{
public:
	Player(int r);
	Player();
	~Player();

	int RandomCompileTimeInt;
	std::vector<float> memshit1;

	virtual const int& GetID() const;
	virtual const int& GetTeam() const;
	virtual const int& GetHealth() const;
	virtual const Vector3& GetPos() const;
	virtual const bool& IsLocalPlayer() const;
	virtual const int& GetDormant() const;

	const DWORD& GetAddress() const;
	void SetAddress(DWORD a);
	void SetIsLocal(bool b);

	virtual void Update(int debug);

	virtual int GetAimEntityID();
	virtual void SetMemoryLocations(UINT_PTR cida[]);

	UINT_PTR Addresses[15];

protected:
	std::string a1;
	std::string a2;
	std::string a3;
	Vector3 m_Position;
	DWORD m_Address;
	std::string a4;
	int m_Team;
	std::string a5;
	int m_Health;
	std::string a6;
	int m_ID;
	bool m_isLocal;
	int m_bDormant;
	UINT_PTR CrosshairIDAddress;

	virtual void SetID(int i);
	virtual void SetTeam(int t);
	virtual void SetHealth(int h);
	virtual void SetPos(Vector3 p);

	virtual void UpdateDormant();

	virtual void UpdateTeam();

	virtual void UpdateID();

	virtual void UpdateHealth();

	virtual void UpdatePosition();
};

#endif
