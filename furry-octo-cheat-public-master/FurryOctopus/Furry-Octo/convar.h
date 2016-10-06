#include "stdafx.h"

using namespace Gdiplus;
using namespace std;


class ConVar
{
public:
	ConVar();
	~ConVar();

	vector<wchar_t*> name;

	void GetValue();
	void SetValue(wchar_t* val);
};