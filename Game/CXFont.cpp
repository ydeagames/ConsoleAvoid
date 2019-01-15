#include "CXFont.h"
#include "CXLib.h"
#include "ScreenManager.h"
#include "Screen.h"

namespace CXLib
{
	namespace Atlas
	{
		constexpr auto TEXTURE_ATLAS_WIDTH = 60; 			// テクスチャアトラスの幅
		constexpr auto TEXTURE_ATLAS_HEIGHT = 27;			// テクスチャアトラスの高さ
		constexpr Vector2 FONT_SPAN_SIZE = { 2.f, 2.f };	// 文字同士の横の空白

		// スプライト
		typedef struct
		{
			char name;
			int x;
			int y;
			int w;
			int h;
		} Sprite;

		// テクスチャアトラス
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

		// フォント・スプライト
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

		// ピクセルを取得
		constexpr char GetPixel(int x, int y)
		{
			return texture_atlas[y][x];
		}
	}

	// フォントハンドルを作成する
	CXFont CreateFontToHandle(CXFontType Font, int Size)
	{
		return{ Font, static_cast<float>(Size) };
	}

	// フォントハンドルを使用した文字列の描画幅(ワールド座標)を取得する
	float GetDrawStringWidthToHandle(LPCWSTR String)
	{
		return (Vector2{ Screen::GetDrawStringLinesWidth(String), 0 }*ConsoleToScreen).x;
	}

	// 文字列描画関数
	void DrawString(Vector2 pos, LPCWSTR String, Color Color)
	{
		Screen::DrawStringLines(pos * ScreenToConsole, Color, String);
	}

	// フォントハンドルを使用した文字列の描画幅(ワールド座標)を取得する
	float GetDrawStringWidthToHandle(LPCWSTR String, const CXFont* FontHandle)
	{
		switch (FontHandle->type)
		{
		default:
		case CXFONT_DEFAULT:
			// 文字数=幅
			return GetDrawStringWidthToHandle(String);
		case CXFONT_PONG:
			// サイズの比
			float size = 7 / FontHandle->size;
			// フォントの幅
			float font_w = 0;
			// フォント左上のX座標
			float font_x = 0;

			// 文字ループ
			for (LPCWSTR c = String; *c != '\0'; c++)
			{
				// スプライト
				const Atlas::Sprite* sprite = nullptr;

				// 改行だったら
				if (*c == '\n')
				{
					// 幅を取得
					font_w = std::max(font_w, font_x);
					// X座標を戻す
					font_x = 0;
					continue;
				}

				// スプライトを取得
				for (auto& font : Atlas::fonts)
				{
					// 一致していたら取得
					if (font.name == *c)
					{
						sprite = &font;
						break;
					}
				}

				// スプライトがあれば幅を加算
				if (sprite != nullptr)
				{
					font_x += sprite->w + Atlas::FONT_SPAN_SIZE.x;
				}
			}
			// フォントの幅の最大
			font_w = std::max(font_w, font_x);
			return font_w;
		}
	}

	// フォントハンドルを使用して文字列を描画する
	void DrawStringToHandle(Vector2 pos, LPCWSTR String, Color Color, const CXFont* FontHandle)
	{
		switch (FontHandle->type)
		{
		default:
		case CXFONT_DEFAULT:
			// 文字列をそのまま描画
			DrawString(pos, String, Color);
			break;
		case CXFONT_PONG:
			// サイズの比
			float size = 7 / FontHandle->size;
			// フォント左上の座標
			Vector2 font_pos = {};
			// フォントの最大高さ
			float font_h = 0;

			// 座標
			Vector2 base_pos = pos * ScreenToConsole;
			// サイズ
			Vector2 base_size = Vector2::one / size * ScreenToConsole;

			// 文字ループ
			for (LPCWSTR c = String; *c != '\0'; c++)
			{
				// スプライト
				const Atlas::Sprite* sprite = nullptr;

				// 改行だったら
				if (*c == '\n')
				{
					// フォント左上のX座標をリセット
					font_pos.x = 0;
					// X座標を戻す
					font_pos.y += font_h + Atlas::FONT_SPAN_SIZE.y;
					// 最大高さのリセット
					font_h = 0;
					continue;
				}

				// スプライトを取得
				for (auto& font : Atlas::fonts)
				{
					// 一致していたら取得
					if (font.name == *c)
					{
						sprite = &font;
						break;
					}
				}

				// スプライトがあれば
				if (sprite != nullptr)
				{
					COORD sprite_pos = font_pos * base_size + base_pos;
					COORD sprite_size = Vector2{ sprite->w, sprite->h }*base_size;

					// Yループ
					for (SHORT iy = 0; iy < sprite_size.Y; iy++)
					{
						// Xループ
						for (SHORT ix = 0; ix < sprite_size.X; ix++)
						{
							// ピクセルが1だったら描画
							if (Atlas::GetPixel(sprite->x + static_cast<int>(ix / base_size.x), sprite->y + static_cast<int>(iy / base_size.y)) == '1')
								Screen::Draw(COORD{ sprite_pos.X + ix, sprite_pos.Y + iy }, Color);
						}
					}

					// フォント左上のX座標を進める
					font_pos.x += sprite->w + Atlas::FONT_SPAN_SIZE.x;
					// フォントの最大高さを更新
					font_h = std::max(font_pos.y, sprite->h / base_size.x);
				}
			}
			break;
		}
	}
}