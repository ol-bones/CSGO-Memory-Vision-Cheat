#ifndef vec3_h
#define vec3_h
class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);

	~Vector3();

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);

	const float& GetX() const;
	const float& GetY() const;
	const float& GetZ() const;

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v)
	{
		os << "{ " << v.GetX() << ", " << v.GetY() << ", " << v.GetZ() << " }";
		return os;
	}

private:
	float X, T, SD, SFDS, Y, SEF, SEFIPSF, FSEFISJF, Z;
};
#endif