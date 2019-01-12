#include "Screen.h"
#include "ScreenManager.h"
#include "ConsoleCharacter.h"

void Screen::Draw(COORD coord, Color textColor)
{
	auto& context = ScreenManager::GetInstance().GetContext();
	if (context.GetBoundsMin().X <= coord.X && coord.X <= context.GetBoundsMax().X - 1 &&
		context.GetBoundsMin().Y <= coord.Y && coord.Y <= context.GetBoundsMax().Y - 1)
	{
		auto& pixel = context.Pixel(coord);
		pixel.Attributes = Attributes{ pixel.Attributes }.back(textColor);
	}
}

void Screen::DrawCharacter(COORD coord, Color textColor, WCHAR ch)
{
	auto& context = ScreenManager::GetInstance().GetContext();
	int width = ConsoleCharacter::GetCharacterWidthCJK(ch);
	if (context.GetBoundsMin().X <= coord.X && coord.X <= context.GetBoundsMax().X - width)
	{
		auto& pixel = context.Pixel(coord);
		pixel.Char.UnicodeChar = ch;
		for (int j = 0; j < width; j++)
		{
			auto& npixel = (&pixel)[j];
			npixel.Attributes = Attributes{ npixel.Attributes }.text(textColor);
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
	auto& context = ScreenManager::GetInstance().GetContext();
	// 右下より左上なら
	if (coord.X < context.GetBoundsMax().X && coord.Y < context.GetBoundsMax().Y)
	{
		// Yループ
		for (SHORT iy = coord.Y; iy < context.GetBoundsMax().Y; iy++)
		{
			// 改行までの距離
			const WCHAR* enter = wcschr(chs, '\n');

			// 左と上にオーバーしすぎて見えなくなっていないかチェック
			if (context.GetBoundsMin().Y <= iy)
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
		const WCHAR* enter = wcschr(chs, '\n');

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
