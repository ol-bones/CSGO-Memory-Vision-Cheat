#pragma once
#include "stdafx.h"
#include <gdiplus.h>

using namespace Gdiplus;

class ESP
{
public:
	ESP();
	~ESP();

	void Draw(Graphics * g, Vector2 dResolution);
};