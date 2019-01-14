#pragma once
#include "Color.h"

class ScreenContext
{
public:
	HANDLE handle;				  	// 出力デバイスのハンドル
	COORD boundsMin;				// バッファーのオフセット
	COORD boundsMax;			  	// バッファサイズ
	WORD attributes;			  	// 属性
	CHAR_INFO default_pixel;	  	// デフォルトピクセル

private:
	SMALL_RECT screenRegion;	  	// 描画領域
	std::vector<CHAR_INFO> buffer;	// バッファ
	CHAR_INFO* bufferPtr;		  	// バッファ

public:
	ScreenContext(const HANDLE& handle, CHAR_INFO default_pixel = { ' ', Attributes{ Colors::Black, Colors::White } });

public:
	void Clear();
	void Flush();

public:
	inline CHAR_INFO& Pixel(COORD coord) { return bufferPtr[coord.X + coord.Y * boundsMax.X]; }
};
