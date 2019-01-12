#pragma once

#include "Color.h"

namespace Screen
{
	void Draw(COORD coord, Color backColor);
	void DrawCharacter(COORD coord, Color textColor, WCHAR ch);
	void DrawString(COORD coord, Color textColor, LPCWSTR chs);
	void DrawStringLines(COORD coord, Color textColor, LPCWSTR chs);

	int GetDrawCharacterWidth(WCHAR ch);
	int GetDrawStringWidth(LPCWSTR chs);
	int GetDrawStringLinesWidth(LPCWSTR ch);
}
