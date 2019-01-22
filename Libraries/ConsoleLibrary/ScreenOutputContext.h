#pragma once
#include "Color.h"

class ScreenOutputContext
{
public:
	HANDLE handle;				  			// 出力デバイスのハンドル
	COORD boundsMin;						// バッファーのオフセット
	COORD boundsMax;			  			// バッファサイズ
	WORD attributes;			  			// 属性
	CHAR_INFO default_pixel;	  			// デフォルトピクセル
	SHORT fontSize;							// フォントサイズ

private:
	SMALL_RECT screenRegion;	  			// 描画領域
	std::vector<CHAR_INFO> dynamicBuffer;	// バッファ
	std::vector<CHAR_INFO> staticBuffer;	// 静的バッファ
	bool useStaticBuffer;					// 静的バッファ
	CHAR_INFO* currentBuffer;		  		// カレントバッファポインタ

public:
	ScreenOutputContext(const HANDLE& handle, CHAR_INFO default_pixel = { ' ', Attributes{ Colors::Black, Colors::White } });

public:
	void Clear();
	void Flush();

public:
	bool HasStaticBuffer();
	void ClearStaticBuffer();
	void BeginStaticBuffer();
	void EndStaticBuffer();
	bool IsStaticBufferMode();

public:
	inline CHAR_INFO& Pixel(COORD coord) { return currentBuffer[coord.X + coord.Y * boundsMax.X]; }
};
