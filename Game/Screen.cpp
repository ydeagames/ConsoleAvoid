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
		pixel.Char.UnicodeChar = L' ';
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
	// �E����荶��Ȃ�
	if (coord.X < context.boundsMax.X && coord.Y < context.boundsMax.Y)
	{
		// Y���[�v
		for (SHORT iy = coord.Y; iy < context.boundsMax.Y; iy++)
		{
			// ���s�܂ł̋���
			LPCWSTR enter = wcschr(chs, '\n');

			// ���Ə�ɃI�[�o�[�������Č����Ȃ��Ȃ��Ă��Ȃ����`�F�b�N
			if (context.boundsMin.Y <= iy)
			{
				DrawString(COORD{ coord.X, iy }, textColor, chs);
			}

			// ���s���Ȃ���΂����ŏI��
			if (enter == nullptr)
				break;
			// ���s������Ή��s�̂��Ƃ܂ŃJ�[�\�������炵�đ��s
			else
				chs = enter + 1;
		}
	}
}

int Screen::GetDrawStringLinesWidth(LPCWSTR chs)
{
	int max = 0;
	// Y���[�v
	for (;;)
	{
		// ���s�܂ł̋���
		LPCWSTR enter = wcschr(chs, '\n');

		int n = GetDrawStringWidth(chs);
		max = std::max(max, n);

		// ���s���Ȃ���΂����ŏI��
		if (enter == nullptr)
			break;
		// ���s������Ή��s�̂��Ƃ܂ŃJ�[�\�������炵�đ��s
		else
			chs = enter + 1;
	}
	return max;
}
