#include "ScreenManager.h"

ScreenManager::ScreenManager()
	: inputContext(ScreenInputContext{ GetStdHandle(STD_INPUT_HANDLE) })
	, outputContext(ScreenOutputContext{ GetStdHandle(STD_OUTPUT_HANDLE) })
{
}

void ScreenManager::UpdateOutputContext()
{
	// スクリーンバッファに関する情報
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	// スクリーンバッファに関する情報の取得
	GetConsoleScreenBufferInfo(outputContext.handle, &screenBufferInfo);

	// スクリーンバッファ修正
	SetConsoleScreenBufferSize(outputContext.handle, COORD{ screenBufferInfo.srWindow.Right + 1, screenBufferInfo.srWindow.Bottom + 1 });

	// コンテキスト更新
	outputContext = ScreenOutputContext{ outputContext.handle, outputContext.default_pixel };
}

bool ScreenManager::SetScreenSize(int width, int height)
{
	// スクリーンバッファのサイズ
	COORD  newSize = COORD{ static_cast<SHORT>(width), static_cast<SHORT>(height) };
	// ウィンドウ矩形
	SMALL_RECT newRect =
	{
		0,				// 左
		0,				// 上
		newSize.X - 1,	// 右
		newSize.Y - 1	// 下
	};

	// 画面のサイズ変更
	if (newSize.X - outputContext.boundsMax.X > 0 || newSize.Y - outputContext.boundsMax.Y > 0)
	{
		// 拡大の場合
		if (!SetConsoleScreenBufferSize(outputContext.handle, newSize))
			return false;

		if (!SetConsoleWindowInfo(outputContext.handle, true, &newRect))
			return false;
	}
	else
	{
		// 縮小の場合
		if (!SetConsoleWindowInfo(outputContext.handle, true, &newRect))
			return false;

		if (!SetConsoleScreenBufferSize(outputContext.handle, newSize))
			return false;
	}

	// コンテキスト更新
	UpdateOutputContext();

	return true;
}

bool ScreenManager::SetWindowSize(int width, int height)
{
	// ローカル変数の宣言
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // フォント情報

	// 現在使用中のフォントの取得
	GetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// コンソールサイズ
	return SetScreenSize(width / fontInfo.dwFontSize.X, height / fontInfo.dwFontSize.Y);
}

void ScreenManager::SetFontSize(int size)
{
	// ローカル変数の宣言
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // フォント情報

	// 現在使用中のフォントの取得
	GetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// フォントサイズの設定
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(size / PixelAspectRatio), static_cast<SHORT>(size) };

	// フォントの更新
	SetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// コンテキスト更新
	UpdateOutputContext();
}

void ScreenManager::SetPixelSize(int size)
{
	// ローカル変数の宣言
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // フォント情報

	// 現在使用中のフォントの取得
	GetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// フォントサイズの設定
	COORD oldFontSize = fontInfo.dwFontSize;

	// フォントサイズの設定
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(size / PixelAspectRatio), static_cast<SHORT>(size) };

	// フォントの更新
	SetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// コンソールサイズ
	SetScreenSize(
		outputContext.boundsMax.X * fontInfo.dwFontSize.X / oldFontSize.X,
		outputContext.boundsMax.Y * fontInfo.dwFontSize.Y / oldFontSize.Y);
}

void ScreenManager::SetDefaultText(WCHAR ch)
{
	auto& pixel = outputContext.default_pixel;
	pixel.Char.UnicodeChar = ch;
}

void ScreenManager::SetDefaultBackground(Color background)
{
	auto& pixel = outputContext.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.back(background);
}

void ScreenManager::SetDefaultForeground(Color foreground)
{
	auto& pixel = outputContext.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.text(foreground);
}

void ScreenManager::SetBackground(Color color)
{
	// 属性設定
	SetConsoleTextAttribute(outputContext.handle, Attributes{ outputContext.attributes }.back(color));

	// コンテキスト更新
	UpdateOutputContext();
}

void ScreenManager::SetForeground(Color color)
{
	// 属性設定
	SetConsoleTextAttribute(outputContext.handle, Attributes{ outputContext.attributes }.text(color));

	// コンテキスト更新
	UpdateOutputContext();
}

void ScreenManager::SetCursorVisibility(bool isVisible)
{
	// ローカル変数の宣言
	CONSOLE_CURSOR_INFO cursorInfo;    // カーソル情報

	// カーソルの表示状態の変更
	GetConsoleCursorInfo(outputContext.handle, &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(outputContext.handle, &cursorInfo);
}

void ScreenManager::SetTitle(LPCWSTR title)
{
	SetConsoleTitleW(title);
}

void ScreenManager::SetCursorPosition(COORD pos)
{
	// カーソル位置の設定
	SetConsoleCursorPosition(outputContext.handle, pos);
}

void ScreenManager::EnableMouseInput()
{
	// 簡易編集モードを無効化
	SetConsoleMode(inputContext.handle, ENABLE_EXTENDED_FLAGS);
	// マウス有効化
	SetConsoleMode(inputContext.handle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}
