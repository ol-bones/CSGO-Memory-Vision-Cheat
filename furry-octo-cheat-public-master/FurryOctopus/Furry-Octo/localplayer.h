#pragma once
#include "stdafx.h"
#include "player.h"
#include "vector3.h"

class LocalPlayer : public Player
{
public:

	LocalPlayer(int r);
	LocalPlayer();
	~LocalPlayer();

	bool m_TriggerToggle = false;

#pragma region Unused Mouse Code
	// this mouse shit doesnt work.
	// msdn and stackoverflow solutions dont work.
	int OriginalMouseSpeed = 0;
	int MouseSpeed = 0, *PTR_MouseSpeed = &MouseSpeed;
	bool Burst = true;

	void ResetMouseSpeed();
	void SetMouseSpeed(int s);
#pragma endregion

	virtual const int& GetID() const;
	virtual const int& GetTeam() const;
	virtual const int& GetHealth() const;
	virtual const Vector3& GetPos() const;
	virtual const bool& IsLocalPlayer() const;
	virtual const int& GetDormant() const;

	virtual void Update(int debug);

	bool IsKeyDown(int Key);
	void LeftClickBurst();

	void LeftClick();
	void SetMemoryLocations(UINT_PTR cida[]);
	virtual int GetAimEntityID();
private:

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