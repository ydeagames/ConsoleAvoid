#include "ScreenOutputContext.h"

ScreenOutputContext::ScreenOutputContext(const HANDLE& hOut, CHAR_INFO default_pixel)
	: handle(hOut)
	, boundsMin(COORD{ 0, 0 })
	, default_pixel(default_pixel)
	, useStaticBuffer(false)
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

	dynamicBuffer = std::vector<CHAR_INFO>(boundsMax.X * boundsMax.Y, default_pixel);
	staticBuffer = std::vector<CHAR_INFO>(boundsMax.X * boundsMax.Y, default_pixel);
	currentBuffer = &dynamicBuffer[0];
}

// 画面を初期化する
void ScreenOutputContext::Clear()
{
	assert(!IsStaticBufferMode() && "still static mode!");
	dynamicBuffer = staticBuffer;
}

void ScreenOutputContext::Flush()
{
	WriteConsoleOutputW(handle, currentBuffer, boundsMax, boundsMin, &screenRegion);
}

bool ScreenOutputContext::HasStaticBuffer()
{
	return useStaticBuffer;
}

void ScreenOutputContext::ClearStaticBuffer()
{
	useStaticBuffer = false;
	// デフォルトピクセルで初期化
	int length = static_cast<int>(boundsMax.X * boundsMax.Y);
	std::fill_n(staticBuffer.begin(), length, default_pixel);
}

void ScreenOutputContext::BeginStaticBuffer()
{
	useStaticBuffer = true;
	currentBuffer = &staticBuffer[0];
}

void ScreenOutputContext::EndStaticBuffer()
{
	currentBuffer = &dynamicBuffer[0];
}

bool ScreenOutputContext::IsStaticBufferMode()
{
	return currentBuffer != &dynamicBuffer[0];
}
