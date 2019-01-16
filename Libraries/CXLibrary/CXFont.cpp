#include "CXFont.h"
#include "CXLib.h"
#include "ScreenManager.h"
#include "Screen.h"

namespace CXLib
{
	namespace Atlas
	{
		constexpr Vector2 FONT_SPAN_SIZE = { 0.f, 1.f };	// �������m�̉��̋�

		struct Character
		{
			int codePoint, x, y, width, height, originX, originY;
		};

		constexpr Character fonts[] =
		{
			{' ', 108, 62, 3, 3, 1, 1},
			{'!', 7, 41, 5, 11, 1, 11},
			{'"', 43, 62, 8, 7, 0, 14},
			{'#', 33, 41, 10, 10, 1, 11},
			{'$', 59, 0, 12, 17, 1, 13},
			{'%', 22, 41, 11, 10, 1, 11},
			{'&', 22, 62, 12, 7, 1, 7},
			{'\'', 75, 62, 5, 5, 1, 11},
			{'(', 22, 0, 8, 18, 0, 14},
			{')', 30, 0, 8, 18, 0, 14},
			{'*', 40, 52, 10, 9, 0, 14},
			{'+', 22, 18, 13, 11, 0, 11},
			{',', 64, 62, 5, 6, 1, 5},
			{'-', 99, 62, 9, 3, 1, 7},
			{'.', 80, 62, 5, 5, 1, 5},
			{'/', 71, 0, 10, 17, 2, 14},
			{'0', 53, 41, 9, 10, 1, 11},
			{'1', 0, 41, 7, 11, 1, 11},
			{'2', 50, 30, 9, 11, 1, 11},
			{'3', 62, 41, 9, 10, 1, 11},
			{'4', 59, 30, 9, 11, 1, 12},
			{'5', 134, 0, 9, 12, 1, 12},
			{'6', 68, 30, 9, 11, 1, 11},
			{'7', 71, 41, 9, 10, 1, 11},
			{'8', 77, 30, 9, 11, 1, 11},
			{'9', 80, 41, 9, 10, 1, 11},
			{':', 12, 41, 5, 11, -1, 10},
			{';', 121, 0, 7, 14, 0, 10},
			{'<', 35, 18, 13, 11, 0, 11},
			{'=', 34, 62, 9, 7, 1, 9},
			{'>', 48, 18, 13, 11, 0, 11},
			{'?', 0, 52, 8, 10, 1, 11},
			{'@', 81, 0, 17, 16, 0, 13},
			{'A', 84, 18, 10, 11, 1, 12},
			{'B', 89, 41, 9, 10, 1, 11},
			{'C', 98, 41, 9, 10, 1, 11},
			{'D', 107, 41, 9, 10, 1, 11},
			{'E', 116, 41, 9, 10, 1, 11},
			{'F', 8, 52, 8, 10, 1, 11},
			{'G', 125, 41, 9, 10, 1, 11},
			{'H', 43, 41, 10, 10, 1, 11},
			{'I', 16, 52, 7, 10, 1, 11},
			{'J', 0, 18, 8, 12, 1, 12},
			{'K', 94, 18, 10, 11, 1, 12},
			{'L', 134, 41, 9, 10, 1, 11},
			{'M', 61, 18, 12, 11, 1, 12},
			{'N', 104, 18, 10, 11, 1, 12},
			{'O', 114, 18, 10, 11, 1, 12},
			{'P', 124, 18, 10, 11, 1, 12},
			{'Q', 110, 0, 11, 14, 1, 12},
			{'R', 134, 18, 10, 11, 1, 12},
			{'S', 86, 30, 9, 11, 1, 12},
			{'T', 143, 41, 9, 10, 1, 11},
			{'U', 0, 30, 10, 11, 1, 12},
			{'V', 73, 18, 11, 11, 1, 12},
			{'W', 8, 18, 14, 11, 1, 12},
			{'X', 10, 30, 10, 11, 1, 12},
			{'Y', 20, 30, 10, 11, 1, 12},
			{'Z', 30, 30, 10, 11, 1, 12},
			{'[', 38, 0, 8, 18, 0, 14},
			{'\\', 98, 0, 12, 14, 1, 13},
			{']', 46, 0, 8, 18, 1, 14},
			{'^', 66, 52, 13, 8, 0, 13},
			{'_', 85, 62, 14, 3, 2, 0},
			{'`', 69, 62, 6, 5, -1, 14},
			{'a', 117, 52, 8, 8, 1, 9},
			{'b', 104, 30, 8, 11, 1, 12},
			{'c', 125, 52, 8, 8, 1, 9},
			{'d', 112, 30, 8, 11, 1, 12},
			{'e', 133, 52, 8, 8, 1, 9},
			{'f', 120, 30, 8, 11, 1, 12},
			{'g', 143, 0, 9, 12, 1, 9},
			{'h', 95, 30, 9, 11, 1, 12},
			{'i', 23, 52, 5, 10, 1, 11},
			{'j', 128, 0, 6, 13, 1, 11},
			{'k', 128, 30, 8, 11, 1, 12},
			{'l', 17, 41, 5, 11, 1, 12},
			{'m', 28, 52, 12, 9, 1, 10},
			{'n', 90, 52, 9, 8, 1, 9},
			{'o', 99, 52, 9, 8, 1, 9},
			{'p', 136, 30, 8, 11, 1, 9},
			{'q', 144, 30, 8, 11, 1, 9},
			{'r', 8, 62, 7, 8, 1, 9},
			{'s', 15, 62, 7, 8, 1, 9},
			{'t', 59, 52, 7, 9, 1, 10},
			{'u', 108, 52, 9, 8, 1, 9},
			{'v', 50, 52, 9, 9, 1, 9},
			{'w', 79, 52, 11, 8, 1, 9},
			{'x', 141, 52, 8, 8, 1, 9},
			{'y', 40, 30, 10, 11, 1, 9},
			{'z', 0, 62, 8, 8, 1, 9},
			{'{', 0, 0, 11, 18, 1, 14},
			{'|', 54, 0, 5, 18, -1, 14},
			{'}', 11, 0, 11, 18, 1, 14},
			{'~', 51, 62, 13, 6, 0, 8},
		};

		CXImage texture_atlas = LoadGraph("Resources/Fonts/font.ppm", Transparent::FirstColor);
	}

	// �t�H���g�n���h�����쐬����
	CXFont CreateFontToHandle(CXFontType Font, int Size)
	{
		return{ Font, static_cast<float>(Size) };
	}

	// �t�H���g�n���h�����g�p����������̕`�敝(���[���h���W)���擾����
	float GetDrawStringWidthToHandle(LPCWSTR String)
	{
		return (Vector2{ Screen::GetDrawStringLinesWidth(String), 0 }*ConsoleToScreen).x;
	}

	// ������`��֐�
	void DrawString(Vector2 pos, LPCWSTR String, Color Color, const Matrix3& world)
	{
		Screen::DrawStringLines(pos * (world * ScreenToConsole), Color, String);
	}

	// �t�H���g�n���h�����g�p����������̕`�敝(���[���h���W)���擾����
	float GetDrawStringWidthToHandle(LPCWSTR String, const CXFont* FontHandle)
	{
		switch (FontHandle->type)
		{
		default:
		case CXFONT_DEFAULT:
			// ������=��
			return GetDrawStringWidthToHandle(String);
		case CXFONT_PONG:
			// �T�C�Y�̔�
			float size = 7 / FontHandle->size;
			// �t�H���g�̕�
			float font_w = 0;
			// �t�H���g�����X���W
			float font_x = 0;

			// �������[�v
			for (LPCWSTR c = String; *c != '\0'; c++)
			{
				// �X�v���C�g
				const Atlas::Character* sprite = nullptr;

				// ���s��������
				if (*c == '\n')
				{
					// �����擾
					font_w = std::max(font_w, font_x);
					// X���W��߂�
					font_x = 0;
					continue;
				}

				// �X�v���C�g���擾
				for (auto& font : Atlas::fonts)
				{
					// ��v���Ă�����擾
					if (font.codePoint == *c)
					{
						sprite = &font;
						break;
					}
				}

				// �X�v���C�g������Ε������Z
				if (sprite != nullptr)
				{
					font_x += sprite->width + Atlas::FONT_SPAN_SIZE.x;
				}
			}
			// �t�H���g�̕��̍ő�
			font_w = std::max(font_w, font_x);
			return font_w;
		}
	}

	// �t�H���g�n���h�����g�p���ĕ������`�悷��
	void DrawStringToHandle(Vector2 pos, LPCWSTR String, Color Color, const CXFont* FontHandle, const Matrix3& world)
	{
		switch (FontHandle->type)
		{
		default:
		case CXFONT_DEFAULT:
			// ����������̂܂ܕ`��
			DrawString(pos, String, Color, world);
			break;
		case CXFONT_PONG:
			Matrix3 matrix = Matrix3::CreateTranslation(pos) * world;
			// �T�C�Y�̔�
			float size = 7 * FontHandle->size;
			// �t�H���g����̍��W
			Vector2 font_pos = {};
			// �t�H���g�̍ő卂��
			float font_h = 0;

			// �������[�v
			for (LPCWSTR c = String; *c != '\0'; c++)
			{
				// �X�v���C�g
				const Atlas::Character* sprite = nullptr;

				// ���s��������
				if (*c == '\n')
				{
					// �t�H���g�����X���W�����Z�b�g
					font_pos.x = 0;
					// X���W��߂�
					font_pos.y += font_h + Atlas::FONT_SPAN_SIZE.y;
					// �ő卂���̃��Z�b�g
					font_h = 0;
					continue;
				}

				// �X�v���C�g���擾
				for (auto& font : Atlas::fonts)
				{
					// ��v���Ă�����擾
					if (font.codePoint == *c)
					{
						sprite = &font;
						break;
					}
				}

				// �X�v���C�g�������
				if (sprite != nullptr)
				{
					auto atlas = Atlas::texture_atlas.DerivationGraph(Vector2{ sprite->x, sprite->y }, Vector2{ sprite->width, sprite->height });
					atlas.DrawGraph(
						Matrix3::CreateScale(Vector2 { sprite->width, sprite->height }) *
						Matrix3::CreateTranslation(-Vector2{ sprite->originX, sprite->originY }) *
						Matrix3::CreateTranslation(font_pos) *
						matrix);

					// �t�H���g�����X���W��i�߂�
					font_pos.x += sprite->width + Atlas::FONT_SPAN_SIZE.x;
					// �t�H���g�̍ő卂�����X�V
					font_h = std::max(font_pos.y, static_cast<float>(sprite->height));
				}
			}
			break;
		}
	}
}