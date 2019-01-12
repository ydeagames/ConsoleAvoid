#pragma once
#include "Color.h"

class ScreenContext
{
private:
	HANDLE hOutput;				  	// 出力デバイスのハンドル
	CHAR_INFO default_pixel;	  	// デフォルトピクセル
	COORD bufferCoord;			  	// バッファーのオフセット
	COORD bufferSize;			  	// バッファサイズ
	SMALL_RECT screenRegion;	  	// 描画領域
	COORD screenSize;			  	// 画面サイズ
	std::vector<CHAR_INFO> buffer;	// バッファ
	CHAR_INFO* bufferPtr;		  	// バッファ
	WORD attributes;			  	// 属性

public:
	ScreenContext(const HANDLE& hOut, CHAR_INFO default_pixel = { ' ', Attributes{ Colors::Black, Colors::White } });

public:
	void Clear();
	void Flush();

public:
	inline CHAR_INFO& Pixel(COORD dwWriteCoord) { return bufferPtr[dwWriteCoord.X + dwWriteCoord.Y * bufferSize.X]; }
	inline CHAR_INFO& DefaultPixel() { return default_pixel; };

public:
	constexpr inline const HANDLE& GetHandle() const { return hOutput; }
	constexpr inline const COORD& GetBoundsMin() const { return bufferCoord; };
	constexpr inline const COORD& GetBoundsMax() const { return bufferSize; };
	constexpr inline const COORD& GetSize() const { return bufferSize; };
	constexpr inline const WORD& GetAttributes() const { return attributes; };
};
