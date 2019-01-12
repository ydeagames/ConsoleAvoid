//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   CXFont.cpp
//!
//! @brief  コンソール・フォント
//!
//! @date   2018/07/29
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================

#include "CXFont.h"
#include "CXLib.h"
#include "MathUtils.h"
#include "ScreenManager.h"
#include <string.h>

// 列挙型の定義 ============================================================

// スプライト
typedef struct
{
	char name;
	int x;
	int y;
	int w;
	int h;
} Sprite;

// 定数の定義 ==============================================================

#define TEXTURE_ATLAS_WIDTH		60		// テクスチャアトラスの幅
#define TEXTURE_ATLAS_HEIGHT	27		// テクスチャアトラスの高さ
#define NUM_FONTS				27		// フォントの数
#define FONT_SPAN_WIDTH			2.f		// 文字同士の横の空白
#define FONT_SPAN_HEIGHT		2.f		// 文字同士の縦の空白

// グローバル変数定義 ======================================================

// テクスチャアトラス
static char* texture_atlas[TEXTURE_ATLAS_HEIGHT] = {
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
static Sprite fonts[NUM_FONTS] = {
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

// フォントハンドルを作成する
CXFont CreateFontToHandle(CXFontType Font, int Size)
{
	return{ Font, (float)Size };
}

// ピクセルを取得
static char GetPixel(int x, int y)
{
	return texture_atlas[y][x];
}

// 文字列描画関数
void DrawString(float x, float y, const WCHAR* String, Attributes Color)
{
	auto& context = ScreenManager::GetInstance().GetContext();

	context.DrawStringLines({ WorldToConsoleX(x), WorldToConsoleY(y) }, Color, String);
}

// フォントハンドルを使用した文字列の描画幅(ワールド座標)を取得する
float GetDrawStringWidthToHandle(const WCHAR* String, const CXFont* FontHandle)
{
	switch (FontHandle->type)
	{
	default:
	case CXFONT_DEFAULT:
		// 文字数=幅
		return ConsoleToWorldX((SHORT)wcslen(String));
		break;
	case CXFONT_PONG:
		// サイズの比
		float size = 7 / FontHandle->size;
		// フォントの幅
		float font_w = 0;
		// フォント左上のX座標
		float font_x = 0;
		// 文字ループ
		const WCHAR* c;
		for (c = String; *c != '\0'; c++)
		{
			// スプライト
			Sprite* sprite = NULL;

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
			{
				int i;
				for (i = 0; i < NUM_FONTS; i++)
				{
					// 一致していたら取得
					if (fonts[i].name == *c)
					{
						sprite = &fonts[i];
						break;
					}
				}
			}

			// スプライトがあれば幅を加算
			if (sprite != NULL)
			{
				font_x += sprite->w + FONT_SPAN_WIDTH;
			}
		}
		// フォントの幅の最大
		font_w = std::max(font_w, font_x);
		return font_w;
		break;
	}
}

// フォントハンドルを使用して文字列を描画する
void DrawStringToHandle(float x, float y, const WCHAR* String, Attributes Color, const CXFont* FontHandle, const WCHAR* Str)
{
	auto& context = ScreenManager::GetInstance().GetContext();

	switch (FontHandle->type)
	{
	default:
	case CXFONT_DEFAULT:
		// 文字列をそのまま描画
		DrawString(x, y, String, Color);
		break;
	case CXFONT_PONG:
		// サイズの比
		float size = 7 / FontHandle->size;
		// フォント左上のX座標
		float font_x = 0;
		// フォント左上のY座標
		float font_y = 0;
		// フォントの最大高さ
		float font_h = 0;
		// 文字ループ
		const WCHAR* c;
		for (c = String; *c != '\0'; c++)
		{
			// スプライト
			Sprite* sprite = NULL;

			// 改行だったら
			if (*c == '\n')
			{
				// フォント左上のX座標をリセット
				font_x = 0;
				// X座標を戻す
				font_y += font_h + FONT_SPAN_HEIGHT;
				// 最大高さのリセット
				font_h = 0;
				continue;
			}

			// スプライトを取得
			{
				int i;
				for (i = 0; i < NUM_FONTS; i++)
				{
					// 一致していたら取得
					if (fonts[i].name == *c)
					{
						sprite = &fonts[i];
						break;
					}
				}
			}
			// スプライトがあれば
			if (sprite != NULL)
			{
				// 文字の幅
				int str_len = std::max(1, (int)wcslen(Str));

				// Yループ
				SHORT ix, iy;
				for (iy = 0; ConsoleToWorldYF(iy) < sprite->h / size; iy += 1)
				{
					// Xループ
					for (ix = 0; ConsoleToWorldXF(ix) < sprite->w / size; ix += str_len)
					{
						// ピクセルが1だったら描画
						if (GetPixel(sprite->x + (int)(ConsoleToWorldX(ix) * size), sprite->y + (int)(ConsoleToWorldY(iy) * size)) == '1')
							context.DrawString({ WorldToConsoleX(font_x / size + x) + ix, WorldToConsoleY(font_y / size + y) + iy }, Color, Str);
					}
				}
				// フォント左上のX座標を進める
				font_x += sprite->w + FONT_SPAN_WIDTH;
				// フォントの最大高さを更新
				font_h = std::max(font_y, ConsoleToWorldY(sprite->h));
			}
		}
		break;
	}
}
