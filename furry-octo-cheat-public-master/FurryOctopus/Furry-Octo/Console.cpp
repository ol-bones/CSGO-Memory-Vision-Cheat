#include "stdafx.h"
#include "Console.h"

using namespace Gdiplus;
using namespace std;

Console::Console()
{
	m_InputStr = vector<wchar_t*>();
	m_Lines = vector<wchar_t*>();
}
Console::~Console() { }
void Console::Open()
{

}

void Console::Close()
{

}

//we thank you microsoft for gifting us with a plethora of obscure string types, amen
void Console::AddLine(wchar_t* key)
{
	m_Lines.push_back(key);
}

void Console::KeyPress(wchar_t* key)
{
	m_InputStr.push_back(key);
}

void Console::Enter()
{
	if (m_InputStr.size() <= 0) { return; }
	wchar_t* str = new wchar_t[1024];
	for (int i = 0; i < 1024; ++i)
	{
		str[i] = wchar_t();
	}
	for (int i = 0; i < m_InputStr.size(); ++i)
	{
		str[i] = *m_InputStr[i];
	}
	m_InputStr.clear();
	this->AddLine(str);
}

void Console::Backspace()
{
	if (m_InputStr.size() <= 0) { return; }
	m_InputStr.pop_back();
}

bool Console::ToggleOpen()
{
	m_ShouldDraw = !m_ShouldDraw;
	return m_ShouldDraw;
}

bool Console::isOpen()
{
	return m_ShouldDraw;
}

void Console::Draw(Graphics * g, Vector2 dResolution)
{
	if (!m_ShouldDraw) { return; }
	Rect clientRect = Rect(0, 0, dResolution.x, dResolution.y);

	Pen * pen = new Pen(Color(255, 255, 0, 0));
	SolidBrush * Grey = new SolidBrush(Color(205, 0, 0, 0));

	SolidBrush * White = new SolidBrush (Color(255, 255, 255, 255));
	LinearGradientBrush * brush2 = new LinearGradientBrush(clientRect, Color::Red, Color::Yellow, LinearGradientMode::LinearGradientModeHorizontal);
	LinearGradientBrush * InnerGradient = new LinearGradientBrush(clientRect, Color(210,0,0,0), Color(215,0,0,0), LinearGradientMode::LinearGradientModeHorizontal);

	Font * font1 = new Font(&FontFamily(L"Times New Roman"), 15, FontStyleRegular, UnitPixel);

	pen->SetWidth(5);

	g->FillRectangle(Grey, Rect(0, 0, dResolution.x, dResolution.y * 0.25));
	g->FillRectangle(InnerGradient, Rect(dResolution.x*0.005, dResolution.y*0.01, dResolution.x*0.99, dResolution.y * 0.20));
	g->FillRectangle(InnerGradient, Rect(dResolution.x*0.005, dResolution.y*0.215, dResolution.x*0.99, dResolution.y * 0.03));

	g->SetTextRenderingHint(TextRenderingHintAntiAlias);
	
	int numLines = 10;
	int yDisplacement = 0;
	int lineStart = (m_Lines.size() <= numLines) ? m_Lines.size() : numLines;
	for (int i = m_Lines.size()-lineStart; i < m_Lines.size(); i++)
	{
		g->DrawString(m_Lines[i], -1, font1, PointF(10, (dResolution.y * 0.01) + (yDisplacement * 22)), White);
		++yDisplacement;
	}

	for (int i = 0; i < m_InputStr.size(); i++)
	{
		g->DrawString(m_InputStr[i], -1, font1, PointF(10+(i*10), (dResolution.y * 0.215)), White);
	}
	delete(pen);
	delete(Grey);
	delete(White);
	delete(brush2);
	delete(InnerGradient);
	delete(font1);
}