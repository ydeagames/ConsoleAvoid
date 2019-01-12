#include "BufferedConsole.h"
#include "ConsoleCharacter.h"

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
	textAttributes = screenBufferInfo.wAttributes;

	buffer = std::vector<CHAR_INFO>(bufferSize.X * bufferSize.Y, default_pixel);
}

// 画面を初期化する
void ScreenContext::Clear()
{
	// オフスクリーンをデフォルトピクセルで初期化
	int length = static_cast<int>(bufferSize.X * bufferSize.Y);
	for (int i = 0; i < length; i++)
		buffer[i] = default_pixel;
}

int ScreenContext::DrawCharacter(COORD dwWriteCoord, Color textColor, WORD wCharacter)
{
	int width = ConsoleCharacter::GetCharacterWidthCJK(wCharacter);
	if (bufferCoord.X <= dwWriteCoord.X && dwWriteCoord.X + width <= bufferSize.X)
	{
		auto pixel = &buffer[dwWriteCoord.X + dwWriteCoord.Y * bufferSize.X];
		pixel[0].Char.UnicodeChar = wCharacter;
		for (int j = 0; j < width; j++)
		{
			auto& npixel = pixel[j];
			npixel.Attributes = Attributes{ npixel.Attributes }.text(textColor);
			if (j != 0)
				npixel.Char.UnicodeChar = L' ';
		}
	}
	return width;
}

void ScreenContext::DrawString(COORD dwWriteCoord, Color textColor, LPCWSTR lpCharacter, DWORD nLength)
{
	int length = static_cast<int>(nLength);
	for (int i = 0, n = 0; length < 0 || i < length; i++)
	{
		auto& chr = lpCharacter[i];
		if (chr == L'\n' || chr == L'\0')
			break;
		int xCoord = n + dwWriteCoord.X;
		int width = DrawCharacter(COORD{ static_cast<SHORT>(xCoord), dwWriteCoord.Y }, textColor, chr);
		n += width;
	}
}

void ScreenContext::DrawStringLines(COORD dwWriteCoord, Color textColor, LPCWSTR lpCharacter)
{
	// 右下より左上なら
	if (dwWriteCoord.X < bufferSize.X && dwWriteCoord.Y < bufferSize.Y)
	{
		// Yループ
		SHORT iy;
		for (iy = dwWriteCoord.Y; iy < bufferSize.Y; iy++)
		{
			// 改行までの距離
			const WCHAR* enter = wcschr(lpCharacter, '\n');

			// 左と上にオーバーしすぎて見えなくなっていないかチェック
			if (iy >= bufferCoord.Y)
				DrawString(COORD{ dwWriteCoord.X, iy }, textColor, lpCharacter);

			// 改行がなければここで終了
			if (enter == nullptr)
				return;
			// 改行があれば改行のあとまでカーソルをずらして続行
			else
				lpCharacter = enter + 1;
		}
	}
}

void ScreenContext::Flush()
{
	WriteConsoleOutput(hOutput, &buffer[0], bufferSize, bufferCoord, &screenRegion);
}
