#include "CXFont.h"
#include "CXLib.h"
#include "ScreenManager.h"
#include "Screen.h"

namespace CXLib
{
	namespace Atlas
	{
		constexpr auto TEXTURE_ATLAS_WIDTH = 60; 			// �e�N�X�`���A�g���X�̕�
		constexpr auto TEXTURE_ATLAS_HEIGHT = 27;			// �e�N�X�`���A�g���X�̍���
		constexpr Vector2 FONT_SPAN_SIZE = { 2.f, 2.f };	// �������m�̉��̋�

		// �X�v���C�g
		typedef struct
		{
			char name;
			int x;
			int y;
			int w;
			int h;
		} Sprite;

		// �e�N�X�`���A�g���X
		constexpr LPCSTR texture_atlas[TEXTURE_ATLAS_HEIGHT] =
		{
			"11   1111  1111  1  1  1111  11111 11111  11111 11111  11111",
			"11   1111  1111  1  1  1111  11111 11111  11111 11111  11111",
			" 1   1  1     1  1  1  1     1        11  1  11 1  11  11  1",
			" 1     11   111  1111  1111  11111    11  11111 11111  111 1",
			" 1    11      1  1111     1  11111    11  11111 11111  1 1 1",
			" 1   11       1     1     1  1  11    11  1  11    11  1  11",
			"111  1111  1111     1  1111  11111    11  11111    11  11111",
			"                                                            ",
			"11111                        11111                          ",
			"11111                        11111                          ",
			"1   1  1111  1111  1111      1     111  1111111  111        ",
			"11111  1  1  1  1  1  1      1 111   1  1  1  1  1 1        ",
			"11111  1  1  1  1  1  1      1 111 111  1  1  1  111        ",
			"1      1  1  1  1  1  1      1  11 1 1  1  1  1  1          ",
			"1      1111  1  1  1111      11111 111  1  1  1  111        ",
			"                      1                                     ",
			"                   1111                                     ",
			"                   1111                                     ",
			"                                                            ",
			"11111 1                1         11  11                     ",
			"11111 1                1         11 11              1       ",
			"  1   1111             1 11       111         1  1  1       ",
			"  1   1  1             111         11         1  1  1       ",
			"  1   1  1             11          1          1  1  1       ",
			"  1   1  1             1 1         1          1  1          ",
			"  1   1  1             1 11        1          1111  1       ",
			"                                                    1       "
		};

		// �t�H���g�E�X�v���C�g
		const std::vector<Sprite> fonts =
		{
			{ '1', 0, 0, 3, 7 },
			{ '2', 5, 0, 4, 7 },
			{ '3', 11, 0, 4, 7 },
			{ '4', 17, 0, 4, 7 },
			{ '5', 23, 0, 4, 7 },
			{ '6', 29, 0, 5, 7 },
			{ '7', 35, 0, 5, 7 },
			{ '8', 42, 0, 5, 7 },
			{ '9', 48, 0, 5, 7 },
			{ '0', 55, 0, 5, 7 },
			{ 'P', 0, 8, 5, 7 },
			{ 'o', 7, 8, 4, 7 },
			{ 'n', 13, 8, 4, 7 },
			{ 'g', 19, 8, 4, 10 },
			{ ' ', 24, 8, 4, 7 },
			{ 'G', 29, 8, 5, 7 },
			{ 'a', 35, 8, 3, 7 },
			{ 'm', 40, 8, 7, 7 },
			{ 'e', 49, 8, 3, 7 },
			{ 'T', 0, 19, 5, 7 },
			{ 'h', 6, 19, 4, 7 },
			{ 'k', 23, 19, 4, 7 },
			{ 'Y', 33, 19, 6, 7 },
			{ 'u', 46, 19, 4, 7 },
			{ '!', 52, 19, 1, 8 }
		};

		// �s�N�Z�����擾
		constexpr char GetPixel(int x, int y)
		{
			return texture_atlas[y][x];
		}
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
	void DrawString(Vector2 pos, LPCWSTR String, Color Color)
	{
		Screen::DrawStringLines(pos * ScreenToConsole, Color, String);
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
				const Atlas::Sprite* sprite = nullptr;

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
					if (font.name == *c)
					{
						sprite = &font;
						break;
					}
				}

				// �X�v���C�g������Ε������Z
				if (sprite != nullptr)
				{
					font_x += sprite->w + Atlas::FONT_SPAN_SIZE.x;
				}
			}
			// �t�H���g�̕��̍ő�
			font_w = std::max(font_w, font_x);
			return font_w;
		}
	}

	// �t�H���g�n���h�����g�p���ĕ������`�悷��
	void DrawStringToHandle(Vector2 pos, LPCWSTR String, Color Color, const CXFont* FontHandle)
	{
		switch (FontHandle->type)
		{
		default:
		case CXFONT_DEFAULT:
			// ����������̂܂ܕ`��
			DrawString(pos, String, Color);
			break;
		case CXFONT_PONG:
			// �T�C�Y�̔�
			float size = 7 / FontHandle->size;
			// �t�H���g����̍��W
			Vector2 font_pos = {};
			// �t�H���g�̍ő卂��
			float font_h = 0;

			// ���W
			Vector2 base_pos = pos * ScreenToConsole;
			// �T�C�Y
			Vector2 base_size = Vector2::one / size * ScreenToConsole;

			// �������[�v
			for (LPCWSTR c = String; *c != '\0'; c++)
			{
				// �X�v���C�g
				const Atlas::Sprite* sprite = nullptr;

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
					if (font.name == *c)
					{
						sprite = &font;
						break;
					}
				}

				// �X�v���C�g�������
				if (sprite != nullptr)
				{
					COORD sprite_pos = font_pos * base_size + base_pos;
					COORD sprite_size = Vector2{ sprite->w, sprite->h }*base_size;

					// Y���[�v
					for (SHORT iy = 0; iy < sprite_size.Y; iy++)
					{
						// X���[�v
						for (SHORT ix = 0; ix < sprite_size.X; ix++)
						{
							// �s�N�Z����1��������`��
							if (Atlas::GetPixel(sprite->x + static_cast<int>(ix / base_size.x), sprite->y + static_cast<int>(iy / base_size.y)) == '1')
								Screen::Draw(COORD{ sprite_pos.X + ix, sprite_pos.Y + iy }, Color);
						}
					}

					// �t�H���g�����X���W��i�߂�
					font_pos.x += sprite->w + Atlas::FONT_SPAN_SIZE.x;
					// �t�H���g�̍ő卂�����X�V
					font_h = std::max(font_pos.y, sprite->h / base_size.x);
				}
			}
			break;
		}
	}
}