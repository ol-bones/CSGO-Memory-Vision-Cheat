#pragma once
#include "stdafx.h"
#include <gdiplus.h>

using namespace Gdiplus;

class Console
{
public:
	Console();
	~Console();

	void Open();
	void Close();

	void Draw(Graphics * g, Vector2 dResolution);
	void AddLine(wchar_t* key);
	void KeyPress(wchar_t* key);
	void Enter();
	void Backspace();
	bool ToggleOpen();
	bool isOpen();
private:
	std::vector<wchar_t*> m_Lines;
	std::vector<wchar_t*> m_InputStr;
	bool m_ShouldDraw = false;
};