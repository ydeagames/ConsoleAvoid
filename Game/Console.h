//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/
//! @file    Console.h
//! @brief   コンソール制御関連のヘッダファイル
//!  
//! @author  S.Takaki
//! @date    2016/7/7
//!
//! @version 1.00
//!
//! Copyright (C) 2016 Susumu Takaki, All rights reserved.
//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/

// 多重インクルードの防止  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//________________________/_____/_____/_____/_____/_____/_____/_____/_____/
#pragma once




// 定数定義    /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//____________/_____/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @enum  KeyCode
//! @brief 仮想キーコード
//===================================================================
enum KeyCode
{
	KEY_BACKSPACE = 0x08,
	KEY_ENTER     = 0x0D, 
	KEY_ESC       = 0x1B,
	KEY_SPACE     = 0x20,
	KEY_UP        = 0x48 | 0x80,
	KEY_LEFT      = 0x4B | 0x80,
	KEY_RIGHT     = 0x4D | 0x80,
	KEY_DOWN      = 0x50 | 0x80,
};


//===================================================================
//! @enum  ConsoleColor
//! @brief 文字色および背景色
//===================================================================
enum ConsoleColor
{
	COLOR_BLACK,
	COLOR_DARK_BLUE,
	COLOR_DARK_GREEN,
	COLOR_DARK_CYAN,
	COLOR_DARK_RED,
	COLOR_DARK_MAGENTA,
	COLOR_DARK_YELLOW,
	COLOR_GRAY,
	COLOR_DARK_GRAY,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
};


//===================================================================
//! @enum  CursorVisibility
//! @brief カーソルの表示状態
//===================================================================
enum CursorVisibility
{
	CURSOR_INVISIBLE,
	CURSOR_VISIBLE
};




// 関数のプロトタイプ宣言  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//________________________/_____/_____/_____/_____/_____/_____/_____/_____/

// コンソール制御関連の関数
void SetScreenSize(int screenWidth, int screenHeight);    // 画面サイズを設定
int  GetScreenWidth(void);                                // 画面幅の取得
int  GetScreenHeight(void);                               // 画面高さの取得
void SetFontSize(int fontSize);                           // フォントサイズの設定
int  GetFontSize(void);                                   // フォントサイズの取得


// カーソル制御用関数
void SetCursorVisibility(CursorVisibility cursorState);                  // カーソルの表示・非表示切り替え
void SetCursorPosition(int cursorPositionX, int cursorPositionY);        // カーソルの位置設定
void GetCursorPosition(int* pCursorPositionX, int* pCursorPositionY);    // カーソルの位置取得
void MoveCursorPosition(int movementX, int movementY);                   // カーソルの移動


// 入力制御用関数
int GetKeyInput(void);                                         // キー入力の取得
int WaitKey(void);                                             // キーが押されるまで待機
int WaitKeyWithMessage(const char* message);                   // キーが押されるまで待機(メッセージ付き)
int WaitKeyWithKeyCode(const int codeList[], int numItems);    // 指定キーの入力待ち
int InputInteger(void);                                        // 整数値の入力


// 出力制御用関数
void ClearScreen(void);                       // 画面のクリア
void SetTextColor(ConsoleColor textColor);    // 文字色の設定
void SetBackColor(ConsoleColor backColor);    // 背景色の設定
