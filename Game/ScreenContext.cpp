#include "ScreenContext.h"

ScreenContext::ScreenContext(const HANDLE& hOut, CHAR_INFO default_pixel)
	: hOutput(hOut)
	, bufferCoord(COORD{ 0, 0 })
	, default_pixel(default_pixel)
{
	// スクリーンバッファに関する情報
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	// スクリーンバッファに関する情報の取得
	GetConsoleScreenBufferInfo(hOutput, &screenBufferInfo);

	// コンソール画面に関する情報の設定
	hOutput = hOutput;
	bufferSize = screenBufferInfo.dwSize;
	screenRegion = { bufferCoord.X, bufferCoord.Y, bufferSize.X - 1, bufferSize.Y - 1 };
	screenSize = { screenBufferInfo.srWindow.Right + 1, screenBufferInfo.srWindow.Bottom + 1 };
	attributes = screenBufferInfo.wAttributes;

	buffer = std::vector<CHAR_INFO>(bufferSize.X * bufferSize.Y, default_pixel);
	bufferPtr = &buffer[0];
}

// 画面を初期化する
void ScreenContext::Clear()
{
	// オフスクリーンをデフォルトピクセルで初期化
	int length = static_cast<int>(bufferSize.X * bufferSize.Y);
	std::fill_n(buffer.begin(), length, default_pixel);
}

void ScreenContext::Flush()
{
	WriteConsoleOutput(hOutput, bufferPtr, bufferSize, bufferCoord, &screenRegion);
}
