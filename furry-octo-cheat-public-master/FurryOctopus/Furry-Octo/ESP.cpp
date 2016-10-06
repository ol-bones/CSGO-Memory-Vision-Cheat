#include "stdafx.h"
#include "ESP.h"

using namespace Gdiplus;
using namespace std;

ESP::ESP()
{

}
ESP::~ESP(){}

void ESP::Draw(Graphics * g, Vector2 dResolution)
{
	Rect clientRect = Rect(0, 0, dResolution.x, dResolution.y);
	
	Pen * red = new Pen(Color(255, 255, 0, 0), 3);

	g->DrawRectangle(red, 200, 200, 200, 200);

	delete(red);
}