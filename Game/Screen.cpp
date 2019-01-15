#include "Screen.h"
#include "ScreenManager.h"
#include "ConsoleCharacter.h"

void Screen::Draw(COORD coord, Color backColor)
{
	static auto& context = ScreenManager::GetInstance().GetContext();
	if (context.boundsMin.X <= coord.X && coord.X <= context.boundsMax.X - 1 &&
		context.boundsMin.Y <= coord.Y && coord.Y <= context.boundsMax.Y - 1)
	{
		auto& pixel = context.Pixel(coord);
		pixel.Attributes = Attributes::back(pixel.Attributes, backColor);
		//auto& pixel2 = context.bufferPtr[coord.X + coord.Y * context.boundsMax.X];
		//pixel2.Attributes = (pixel2.Attributes & 0x0f) | ((backColor & 0xf) << 4);
	}
}

void Screen::DrawCharacter(COORD coord, Color textColor, WCHAR ch)
{
	static auto& context = ScreenManager::GetInstance().GetContext();
	int width = ConsoleCharacter::GetCharacterWidthCJK(ch);
	if (context.boundsMin.X <= coord.X && coord.X <= context.boundsMax.X - width)
	{
		auto& pixel = context.Pixel(coord);
		pixel.Char.UnicodeChar = ch;
		for (int j = 0; j < width; j++)
		{
			auto& npixel = (&pixel)[j];
			npixel.Attributes = Attributes::text(npixel.Attributes, textColor);
			if (j != 0)
				npixel.Char.UnicodeChar = L' ';
		}
	}
}

int Screen::GetDrawCharacterWidth(WCHAR ch)
{
	return ConsoleCharacter::GetCharacterWidthCJK(ch);
}

void Screen::DrawString(COORD coord, Color textColor, LPCWSTR chs)
{
	int n = 0;
	for (int i = 0;; i++)
	{
		auto& chr = chs[i];
		if (chr == L'\n' || chr == L'\0')
			break;
		int xCoord = n + coord.X;
		DrawCharacter(COORD{ static_cast<SHORT>(xCoord), coord.Y }, textColor, chr);
		n += GetDrawCharacterWidth(chr);
	}
}

int Screen::GetDrawStringWidth(LPCWSTR chs)
{
	int n = 0;
	for (int i = 0;; i++)
	{
		auto& chr = chs[i];
		if (chr == L'\n' || chr == L'\0')
			break;
		n += GetDrawCharacterWidth(chr);
	}
	return n;
}

void Screen::DrawStringLines(COORD coord, Color textColor, LPCWSTR chs)
{
	static auto& context = ScreenManager::GetInstance().GetContext();
	// 右下より左上なら
	if (coord.X < context.boundsMax.X && coord.Y < context.boundsMax.Y)
	{
		// Yループ
		for (SHORT iy = coord.Y; iy < context.boundsMax.Y; iy++)
		{
			// 改行までの距離
			LPCWSTR enter = wcschr(chs, '\n');

			// 左と上にオーバーしすぎて見えなくなっていないかチェック
			if (context.boundsMin.Y <= iy)
			{
				DrawString(COORD{ coord.X, iy }, textColor, chs);
			}

			// 改行がなければここで終了
			if (enter == nullptr)
				break;
			// 改行があれば改行のあとまでカーソルをずらして続行
			else
				chs = enter + 1;
		}
	}
}

int Screen::GetDrawStringLinesWidth(LPCWSTR chs)
{
	int max = 0;
	// Yループ
	for (;;)
	{
		// 改行までの距離
		LPCWSTR enter = wcschr(chs, '\n');

		int n = GetDrawStringWidth(chs);
		max = std::max(max, n);

		// 改行がなければここで終了
		if (enter == nullptr)
			break;
		// 改行があれば改行のあとまでカーソルをずらして続行
		else
			chs = enter + 1;
	}
	return max;
}
