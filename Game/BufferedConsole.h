#pragma once

#include "Color.h"

class ScreenContext
{
public:
	HANDLE hOutput;					// 出力デバイスのハンドル
	CHAR_INFO default_pixel;		// デフォルトピクセル
	COORD bufferCoord;				// バッファーのオフセット
	COORD bufferSize;				// バッファサイズ
	SMALL_RECT screenRegion;		// 描画領域
	COORD screenSize;				// 画面サイズ

private:
	WORD textAttributes;			// 文字の前景色属性と背景色属性
	std::vector<CHAR_INFO> buffer;	// バッファ

public:
	ScreenContext(const HANDLE& hOut, CHAR_INFO default_pixel = { ' ', Attributes{ Colors::Black, Colors::White } });

public:
	void Clear();
	int DrawCharacter(COORD dwWriteCoord, Color textColor, WORD wCharacter);
	void DrawString(COORD dwWriteCoord, Color textColor, LPCWSTR lpCharacter, DWORD nLength = -1);
	void DrawStringLines(COORD dwWriteCoord, Color textColor, LPCWSTR lpCharacter);
	void Flush();
};
