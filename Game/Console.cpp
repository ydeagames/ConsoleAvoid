//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/
//! @file    Console.cpp
//! @brief   コンソール制御関連のソースファイル
//!  
//! @author  S.Takaki
//! @date    2018/7/7
//!
//! @version 1.00
//!
//! Copyright (C) 2018 Susumu Takaki, All rights reserved.
//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/

// ヘッダファイルのインクルード  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//______________________________/_____/_____/_____/_____/_____/_____/_____/
#include "Console.h"    // コンソール制御関数

#include <stdio.h>      // printf()
#include <string.h>     // strlen()
#include <conio.h>      // _kbhit(), _getch(), _putch()
#include <windows.h>    // Windows制御関連




// 構造体の定義      /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//__________________/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @brief 画面に関連する情報
//===================================================================
struct tag_ScreenInfo
{
	HANDLE hOutput;           // 出力デバイスのハンドル
	COORD  bufferSize;        // バッファサイズ
	COORD  screenSize;        // 画面サイズ
	WORD   textAttributes;    // 文字の前景色属性と背景色属性
};
typedef struct tag_ScreenInfo ScreenContext;




// 非公開関数のプロトタイプ宣言  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//______________________________/_____/_____/_____/_____/_____/_____/_____/
ScreenContext GetCurrentConsoleScreenInfo(void);    // コンソール画面の初期化
void ResizeScreen(SHORT dx, SHORT dy);           // 画面のサイズ変更
int  Clamp(int num, int min, int max);           // 指定された範囲内の数値を返す
void ExitWithMessage(const char* message);       // 強制終了(メッセージ付き)




// ファイルスコープ変数の宣言    /__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//______________________________/_____/_____/_____/_____/_____/_____/_____/
static ScreenContext s_screenInfo = GetCurrentConsoleScreenInfo();    // 画面に関連する情報




// 公開関数の定義    /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//__________________/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @brief 画面サイズを設定
//!
//! @param[in] screenWidth  画面の幅
//! @param[in] screenHeight 画面の高さ
//!
//! @return なし
//===================================================================
void SetScreenSize(int screenWidth, int screenHeight)
{
	// ローカル変数の宣言 --------------------------------
	SHORT dx = screenWidth  - s_screenInfo.screenSize.X;    // 水平方向の差分
	SHORT dy = screenHeight - s_screenInfo.screenSize.Y;    // 垂直方向の差分


	// 水平方向のスケーリング ------------------------------
	ResizeScreen(dx, 0);


	// 垂直方向のスケーリング ------------------------------
	ResizeScreen(0, dy);
}



//===================================================================
//! @brief 画面幅の取得
//!
//! @param[in] なし
//!
//! @return 画面幅(半角文字単位)
//===================================================================
int GetScreenWidth(void)
{
	return s_screenInfo.screenSize.X;
}



//===================================================================
//! @brief 画面高さの取得
//!
//! @param[in] なし
//!
//! @return 画面高さ(半角文字単位)
//===================================================================
int GetScreenHeight(void)
{
	return s_screenInfo.screenSize.Y;
}



//===================================================================
//! @brief フォントサイズの設定
//!        (半角のため横方向は半分になる)
//!
//! @param[in] fontSize フォントサイズ[pixel]
//!
//! @return なし
//===================================================================
void SetFontSize(int fontSize)
{
	// ローカル変数の宣言 --------------------------------
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(fontInfo) };    // フォント情報


	// 現在使用中のフォントの取得 ------------------------
	if (GetCurrentConsoleFontEx(s_screenInfo.hOutput, false, &fontInfo) == 0)
	{
		ExitWithMessage("フォントに関する情報の取得に失敗しました");
	}


	// フォントサイズの設定 ------------------------------
	fontInfo.dwFontSize.X = (SHORT)fontSize / 2;
	fontInfo.dwFontSize.Y = (SHORT)fontSize;


	// フォントの更新 ------------------------------------
	if (SetCurrentConsoleFontEx(s_screenInfo.hOutput, false, &fontInfo) == 0)
	{
		ExitWithMessage("フォントの更新に失敗しました");
	}
}



//===================================================================
//! @brief フォントサイズ(高さ)の取得
//!
//! @param[in] なし
//!
//! @return フォントサイズ[pixel]
//===================================================================
int GetFontSize(void)
{
	// ローカル変数の宣言 --------------------------------
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(fontInfo) };    // フォント情報


	// 現在使用中のフォントの取得 ------------------------
	if (GetCurrentConsoleFontEx(s_screenInfo.hOutput, false, &fontInfo) == 0)
	{
		ExitWithMessage("フォントに関する情報の取得に失敗しました");
	}


	// フォントサイズの返却 ------------------------------
	return (int)fontInfo.dwFontSize.Y;
}



//===================================================================
//! @brief カーソルの表示・非表示切り替え
//!
//! @param[in] cursorState カーソルの表示状態
//!
//! @return なし
//===================================================================
void SetCursorVisibility(CursorVisibility cursorState)
{
	// ローカル変数の宣言 --------------------------------
	CONSOLE_CURSOR_INFO cursorInfo;    // カーソル情報


	// カーソルの表示状態の変更 --------------------------
	if (GetConsoleCursorInfo(s_screenInfo.hOutput, &cursorInfo) == 0)
	{
		ExitWithMessage("カーソルに関する情報の取得に失敗しました");
	}
	cursorInfo.bVisible = cursorState;


	// カーソルの表示状態の更新
	if (SetConsoleCursorInfo(s_screenInfo.hOutput, &cursorInfo) == 0)
	{
		ExitWithMessage("カーソルに関する情報の更新に失敗しました");
	}
}



//===================================================================
//! @brief カーソルの位置設定
//!
//! @param[in] cursorPositionX カーソルのX座標
//! @param[in] cursorPositionY カーソルのY座標
//!
//! @return なし
//===================================================================
void SetCursorPosition(int cursorPositionX, int cursorPositionY)
{
	// ローカル変数の宣言 --------------------------------
	COORD  cursorPosition =    // カーソルの位置情報
	{
		(SHORT)cursorPositionX,    // X座標
		(SHORT)cursorPositionY     // Y座標
	};


	// カーソル位置の設定 ---------------------------------
	SetConsoleCursorPosition(s_screenInfo.hOutput, cursorPosition);
}



//===================================================================
//! @brief カーソルの座標取得
//!
//! @param[out] pCursorPositionX カーソルのX座標を格納する変数へのポインタ
//! @param[out] pCursorPositionY カーソルのY座標を格納する変数へのポインタ
//!
//! @return なし
//===================================================================
void GetCursorPosition(int* pCursorPositionX, int* pCursorPositionY)
{
	// ローカル変数の宣言 --------------------------------
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;    // スクリーンバッファに関する情報


	// スクリーンバッファに関する情報の取得 --------------
	if (GetConsoleScreenBufferInfo(s_screenInfo.hOutput, &screenBufferInfo) == 0)
	{
		ExitWithMessage("スクリーンバッファに関する情報の取得に失敗しました");
	}


	// カーソル位置の書き込み ----------------------------
	*pCursorPositionX = screenBufferInfo.dwCursorPosition.X;
	*pCursorPositionY = screenBufferInfo.dwCursorPosition.Y;
}



//===================================================================
//! @brief カーソルの移動
//!
//! @param[in] movementX X方向の移動量
//! @param[in] movementY Y方向の移動量
//!
//! @return なし
//===================================================================
void MoveCursorPosition(int movementX, int movementY)
{
	// ローカル変数の宣言 --------------------------------
	int cursorPositionX;    // カーソルのX座標
	int cursorPositionY;    // カーソルのY座標


	// カーソル位置の設定 --------------------------------
	GetCursorPosition(&cursorPositionX, &cursorPositionY);
	SetCursorPosition((cursorPositionX + movementX), (cursorPositionY + movementY));
}



//===================================================================
//! @brief キー入力の取得
//!
//! @param[in] なし
//!
//! @retval キーコード キー入力あり(特殊キーの場合は仮想キーコードを返却)
//! @retval 0          キー入力なし
//===================================================================
int GetKeyInput(void)
{
	// ローカル変数の宣言 --------------------------------
	int code = 0;    // 入力されたキーのコード


	// 入力されたキーの取得 ------------------------------
	if (_kbhit())
	{
		code = _getch();    // 入力キーの取得

		if (code == 0xE0 || code == 0x00)    // 特殊文字の場合
		{
    		code = _getch() | 0x80;
		}
	}


	// 文字コードの返却 ----------------------------------
	return code;
}



//===================================================================
//! @brief キーが押されるまで待機
//!
//! @param[in] なし
//!
//! @return 入力されたキーのコードを返却
//===================================================================
int WaitKey(void)
{
	// ローカル変数の宣言 --------------------------------
	int code;    // 入力されたキーのコード


	// 入力処理 ------------------------------------------
	while (1)
	{
		code = GetKeyInput();    // 入力キーの取得

		// 入力条件の合致判定 --------------------------------
		if (code != 0)
		{
			return code;
		}
	}
}



//===================================================================
//! @brief キーが押されるまで待機(メッセージ付き)
//!
//! @param[in] //! @param[in] message 出力メッセージ
//!
//! @return 入力されたキーのコードを返却
//===================================================================
int WaitKeyWithMessage(const char* message)
{
	// メッセージの表示
	printf("%s", message);


	// 入力処理 ------------------------------------------
	return WaitKey();
}



//===================================================================
//! @brief 指定されたキーが押されるまで待機
//!
//! @param[in] codeList キーコードのリスト
//! @param[in] numItems リストのアイテム数
//!
//! @return 選択肢の中から入力されたキーのコードを返却
//===================================================================
int WaitKeyWithKeyCode(const int codeList[], int numItems)
{
	// ローカル変数の宣言 --------------------------------
    int code;    // 入力されたキーのコード
    int i;       // ループ用

	
	// 入力処理 ------------------------------------------
    while (1)
	{
	    code = GetKeyInput();    // 入力キーの取得

	    // 入力条件の合致判定 --------------------------------
		if (code != 0)
		{
			for (i = 0; i < numItems; i++)
			{
				if (code == codeList[i])
				{
					return codeList[i];
				}
			}
		}
    }
}



//===================================================================
//! @brief 整数値の入力
//!
//! @param[in] なし
//!
//! @return 入力された整数値を返却
//===================================================================
int InputInteger(void)
{
	// ローカル変数の宣言 --------------------------------
	int digit  = 0;    // 桁
    int number = 0;    // 入力値
	int code;          // 入力されたキーのコード
	

    // 入力処理 ------------------------------------------
    while (1)
	{
	    code = GetKeyInput();    // 入力キーの取得

		if (code >= '0' && code <= '9')
		{
			digit++;
			_putch(code);
			number = number * 10 + ( code - '0' );
		}
		else if (code == KEY_ENTER)
		{
			_putch('\n');
			break;
		}
		else if (code == KEY_BACKSPACE)
		{
			if (digit > 0)
			{
				digit--;
				number /= 10;
				_putch(KEY_BACKSPACE);
				_putch(KEY_SPACE);
				_putch(KEY_BACKSPACE);
			}
		}
    }


	// 入力された整数値を返却 ----------------------------
	return number;
}



//===================================================================
//! @brief 画面のクリア
//!
//! @param[in] なし
//!
//! @return なし
//===================================================================
void ClearScreen(void)
{
	// ローカル変数の宣言 --------------------------------
	DWORD length     = s_screenInfo.bufferSize.X * s_screenInfo.bufferSize.Y;    // 書き込む文字数
	COORD writeCoord = {0, 0};                                                   // 文字を書き込むスクリーンバッファ座標
	DWORD numCharactersWritten;                                                  // スクリーンバッファに実際に書き込まれた文字数


	// 画面をクリア --------------------------------------
	FillConsoleOutputCharacter(s_screenInfo.hOutput, ' ', length, writeCoord, &numCharactersWritten);
	FillConsoleOutputAttribute(s_screenInfo.hOutput, s_screenInfo.textAttributes, length, writeCoord, &numCharactersWritten);

	SetCursorPosition(0, 0);
}



//===================================================================
//! @brief 文字色の設定
//!
//! @param[in] textColor 文字色
//!
//! @return なし
//===================================================================
void SetTextColor(ConsoleColor textColor)
{
	// 文字色の変更--------------------------------------
	s_screenInfo.textAttributes = (s_screenInfo.textAttributes & 0xF0) | textColor;
    

	// 文字の属性の更新 ----------------------------------
	if (SetConsoleTextAttribute(s_screenInfo.hOutput, s_screenInfo.textAttributes) == 0)
	{
		ExitWithMessage("文字色の変更に失敗しました。");
	}
}



//===================================================================
//! @brief 背景色の設定
//!
//! @param[in] backColor 背景色
//!
//! @return なし
//===================================================================
void SetBackColor(ConsoleColor backColor)
{
	// 背景色の変更 --------------------------------------
	s_screenInfo.textAttributes = (s_screenInfo.textAttributes & 0x0F) | (backColor << 4);


	// 文字の属性の更新 ----------------------------------
	if (SetConsoleTextAttribute(s_screenInfo.hOutput, s_screenInfo.textAttributes) == 0)
	{
		ExitWithMessage("背景色の変更に失敗しました。");
	}
}




// 非公開関数の定義  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//__________________/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @brief 現在のコンソール画面に関する情報を取得
//!
//! @param[in] なし
//!
//! @return 現在のコンソール画面に関する情報
//===================================================================
ScreenContext GetCurrentConsoleScreenInfo(void)
{
	// ローカル変数の宣言 --------------------------------
	HANDLE                     hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);    // 標準出力デバイスのハンドル
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;                                // スクリーンバッファに関する情報
	ScreenContext                 screenInfo;                                      // コンソール画面に関する情報
	

	// スクリーンバッファに関する情報の取得 --------------
	if (GetConsoleScreenBufferInfo(hStdOutput, &screenBufferInfo) == 0)
	{
		ExitWithMessage("スクリーンバッファに関する情報の取得に失敗しました");
	}


	// コンソール画面に関する情報の設定 ------------------
	screenInfo.hOutput        = hStdOutput;
	screenInfo.bufferSize     = screenBufferInfo.dwSize;
	screenInfo.screenSize.X   = screenBufferInfo.srWindow.Right  + 1;
	screenInfo.screenSize.Y   = screenBufferInfo.srWindow.Bottom + 1;
	screenInfo.textAttributes = screenBufferInfo.wAttributes;


	// コンソール画面に関する情報の返却 ------------------
	return screenInfo;
}



//===================================================================
//! @brief 画面の拡大・縮小
//!
//! @param[in] dx 水平方向の差分
//! @param[in] dy 垂直方向の差分
//!
//! @return なし
//===================================================================
void ResizeScreen(SHORT dx, SHORT dy)
{
	// ローカル変数の宣言 --------------------------------
	COORD  screenBufferSize =    // スクリーンバッファのサイズ
	{
		s_screenInfo.screenSize.X + dx,    // 幅
		s_screenInfo.screenSize.Y + dy     // 高さ
	};

	SMALL_RECT windowRect =    // ウィンドウ矩形
	{
		0,                         // 左
		0,                         // 上
		screenBufferSize.X - 1,    // 右
		screenBufferSize.Y - 1     // 下
	};


	// 画面のサイズ変更 ----------------------------------
	if (dx > 0 || dy > 0)
	{
		// 拡大の場合 --------------------------------
		if (!SetConsoleScreenBufferSize(s_screenInfo.hOutput, screenBufferSize))
		{
			ExitWithMessage("スクリーンバッファのサイズの変更に失敗しました。");
		}

		if (!SetConsoleWindowInfo(s_screenInfo.hOutput, true, &windowRect))
		{
			ExitWithMessage("ウィンドウのサイズの変更に失敗しました。");
		}
	}
	else
	{
		// 縮小の場合 --------------------------------
		if (!SetConsoleWindowInfo(s_screenInfo.hOutput, true, &windowRect))
		{
			ExitWithMessage("ウィンドウのサイズの変更に失敗しました。");
		}

		if (!SetConsoleScreenBufferSize(s_screenInfo.hOutput, screenBufferSize))
		{
			ExitWithMessage("スクリーンバッファのサイズの変更に失敗しました。");
		}
	}


	// 画面に関する情報内のサイズ情報を更新
	s_screenInfo.screenSize = screenBufferSize;
	s_screenInfo.bufferSize = screenBufferSize;
}



//===================================================================
//! @brief 指定された範囲内の数値を返す
//!
//! @param[in] num 判定する数値
//! @param[in] min 範囲の最小値
//! @param[in] max 範囲の最大値
//!
//! @return 指定された範囲内の数値
//===================================================================
int Clamp(int num, int min, int max)
{
	return (num < min) ? min : ((num < max) ? num : max);
}



//===================================================================
//! @brief 強制終了(メッセージ付き)
//!
//! @param[in] message 出力メッセージ
//!
//! @return なし
//===================================================================
void ExitWithMessage(const char* message)
{
	WaitKeyWithMessage(message);

	exit(-1);
}
