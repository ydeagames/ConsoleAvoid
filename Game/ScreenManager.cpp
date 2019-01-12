#include "ScreenManager.h"

ScreenManager::ScreenManager()
	: context(ScreenContext{ GetStdHandle(STD_OUTPUT_HANDLE) })
{
}

void ScreenManager::UpdateContext()
{
	context = ScreenContext{ context.handle, context.default_pixel };
}

bool ScreenManager::SetScreenSize(int width, int height)
{
	// スクリーンバッファのサイズ
	COORD  newSize = COORD{ static_cast<SHORT>(width), static_cast<SHORT>(height) };
	// ウィンドウ矩形
	SMALL_RECT newRect =
	{
		0,                         // 左
		0,                         // 上
		newSize.X - 1,    // 右
		newSize.Y - 1     // 下
	};


	// 画面のサイズ変更 ----------------------------------
	if (newSize.X - context.boundsMax.X > 0 || newSize.Y - context.boundsMax.Y > 0)
	{
		// 拡大の場合 --------------------------------
		if (!SetConsoleScreenBufferSize(context.handle, newSize))
			return false;

		if (!SetConsoleWindowInfo(context.handle, true, &newRect))
			return false;
	}
	else
	{
		// 縮小の場合 --------------------------------
		if (!SetConsoleWindowInfo(context.handle, true, &newRect))
			return false;

		if (!SetConsoleScreenBufferSize(context.handle, newSize))
			return false;
	}

	// コンテキスト更新
	UpdateContext();

	return true;
}

bool ScreenManager::SetWindowSize(int width, int height)
{
	// ローカル変数の宣言
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // フォント情報

	// 現在使用中のフォントの取得
	GetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// コンソールサイズ
	return SetScreenSize(width / fontInfo.dwFontSize.X, height / fontInfo.dwFontSize.Y);
}

void ScreenManager::SetFontSize(int width, int height)
{
	// ローカル変数の宣言
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // フォント情報

	// 現在使用中のフォントの取得
	GetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// フォントサイズの設定
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(width), static_cast<SHORT>(height) };

	// フォントの更新
	SetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// コンテキスト更新
	UpdateContext();
}

void ScreenManager::SetPixelSize(int width, int height)
{
	// ローカル変数の宣言
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // フォント情報

	// 現在使用中のフォントの取得
	GetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// フォントサイズの設定
	COORD oldFontSize = fontInfo.dwFontSize;

	// フォントサイズの設定
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(width), static_cast<SHORT>(height) };

	// フォントの更新
	SetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// コンソールサイズ
	SetScreenSize(
		context.boundsMax.X * fontInfo.dwFontSize.X / oldFontSize.X,
		context.boundsMax.Y * fontInfo.dwFontSize.Y / oldFontSize.Y);
}

void ScreenManager::SetDefaultText(WCHAR ch)
{
	auto& pixel = context.default_pixel;
	pixel.Char.UnicodeChar = ch;
}

void ScreenManager::SetDefaultBackground(Color background)
{
	auto& pixel = context.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.back(background);
}

void ScreenManager::SetDefaultForeground(Color foreground)
{
	auto& pixel = context.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.text(foreground);
}

void ScreenManager::SetBackground(Color color)
{
	// 属性設定
	SetConsoleTextAttribute(context.handle, Attributes{ context.attributes }.back(color));

	// コンテキスト更新
	UpdateContext();
}

void ScreenManager::SetForeground(Color color)
{
	// 属性設定
	SetConsoleTextAttribute(context.handle, Attributes{ context.attributes }.text(color));

	// コンテキスト更新
	UpdateContext();
}

void ScreenManager::SetCursorVisibility(bool isVisible)
{
	// ローカル変数の宣言
	CONSOLE_CURSOR_INFO cursorInfo;    // カーソル情報

	// カーソルの表示状態の変更
	GetConsoleCursorInfo(context.handle, &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(context.handle, &cursorInfo);
}