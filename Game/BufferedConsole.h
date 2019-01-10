//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   BufferedConsole.h
//!
//! @brief  ソフトウェア・コンソールバッファ
//!
//! @date   2018/07/29
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================

#include "Console.h"

// 型の定義 ================================================================

// コンソール座標の要素
typedef short SHORT;

// 構造体の定義 ============================================================

// 属性(色)
class Attributes
{
public:
	ConsoleColor foreground;
	ConsoleColor background;

public:
	// 属性(色)を作成
	Attributes(ConsoleColor foreground, ConsoleColor background);
	Attributes(const WORD& word);
	operator WORD() const;
};

class ScreenInfo
{
public:
	HANDLE hOutput;					// 出力デバイスのハンドル
	CHAR_INFO default_pixel;		// デフォルトピクセル
	COORD bufferCoord;				// バッファーのオフセット
	COORD bufferSize;				// バッファサイズ
	SMALL_RECT screenRegion;		// 描画領域
	COORD  screenSize;				// 画面サイズ

private:
	WORD   textAttributes;			// 文字の前景色属性と背景色属性
	std::vector<CHAR_INFO> buffer;	// バッファ

public:
	ScreenInfo(const HANDLE& hOut, CHAR_INFO default_pixel = { ' ', Attributes{ ConsoleColor::COLOR_WHITE, ConsoleColor::COLOR_BLACK } });

public:
	void Clear();
	void Draw(COORD dwWriteCoord, WORD wAttribute, LPCWSTR lpCharacter, DWORD nLength);
	//void DrawA(COORD dwWriteCoord, WORD wAttribute, LPCSTR lpCharacter, DWORD nLength);
	void Flush();
};

// グローバル変数宣言 ======================================================

// デフォルト属性(色)
extern const Attributes DEFAULT_ATTR;

// 関数の宣言 ==============================================================


// ソフトウェア・コンソールバッファを初期化する
void BufferedConsole_Initialize(void);

// 画面を初期化する
void Clear(void);

// 指定座標に指定文字を指定属性(色)で表示
void Print(COORD coord, Attributes attributes, const WCHAR* format);

// スクリーンを反映
void BufferedConsole_Flush(void);
