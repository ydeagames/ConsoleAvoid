#include "ScreenContext.h"

ScreenContext::ScreenContext(const HANDLE& hOut, CHAR_INFO default_pixel)
	: handle(hOut)
	, boundsMin(COORD{ 0, 0 })
	, default_pixel(default_pixel)
{
	// スクリーンバッファに関する情報
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	// スクリーンバッファに関する情報の取得
	GetConsoleScreenBufferInfo(handle, &screenBufferInfo);

	// コンソール画面に関する情報の設定
	handle = handle;
	boundsMax = screenBufferInfo.dwSize;
	screenRegion = { boundsMin.X, boundsMin.Y, boundsMax.X - 1, boundsMax.Y - 1 };
	attributes = screenBufferInfo.wAttributes;

	buffer = std::vector<CHAR_INFO>(boundsMax.X * boundsMax.Y, default_pixel);
	bufferPtr = &buffer[0];
}

// 画面を初期化する
void ScreenContext::Clear()
{
	// オフスクリーンをデフォルトピクセルで初期化
	int length = static_cast<int>(boundsMax.X * boundsMax.Y);
	std::fill_n(buffer.begin(), length, default_pixel);
}

void ScreenContext::Flush()
{
	WriteConsoleOutputW(handle, bufferPtr, boundsMax, boundsMin, &screenRegion);
}
