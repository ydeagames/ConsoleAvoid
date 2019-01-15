#pragma once

namespace ConsoleCharacter
{
	int GetCharacterWidth(wchar_t ucs);
	int GetStringWidth(const wchar_t* pwcs, size_t n);
	int GetCharacterWidthCJK(wchar_t ucs);
	int GetStringWidthCJK(const wchar_t* pwcs, size_t n);
}

