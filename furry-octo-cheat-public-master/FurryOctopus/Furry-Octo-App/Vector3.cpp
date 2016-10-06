#include "stdafx.h"
#include "vector3.h"
using namespace std;

	Vector3::Vector3() { };

	Vector3::Vector3(float x, float y, float z)
	{
		this->SetX(x);
		this->SetY(y);
		this->SetZ(z);
	}

	Vector3::~Vector3() { };

	void Vector3::SetX(float x) { this->X = x; };
	void Vector3::SetY(float y) { this->Y = y; };
	void Vector3::SetZ(float z) { this->Z = z; };

	const float& Vector3::GetX() const { return X; };
	const float& Vector3::GetY() const { return Y; };
	const float& Vector3::GetZ() const { return Z; };