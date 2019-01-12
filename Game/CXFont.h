//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   CXFont.h
//!
//! @brief  コンソール・フォント
//!
//! @date   2018/07/29
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================

#include "CXLib.h"

// 列挙型の定義 ============================================================

// フォントの種類
typedef enum {
	CXFONT_DEFAULT,
	CXFONT_PONG
} CXFontType;

// 構造体の定義 ============================================================

// フォント
typedef struct {
	CXFontType type;
	float size;
} CXFont;

// 関数の宣言 ==============================================================

// フォントハンドルを作成する
CXFont CreateFontToHandle(CXFontType Font, int Size);

// 文字列描画関数
void DrawString(float x, float y, const WCHAR* String, Color Color);

// フォントハンドルを使用した文字列の描画幅(ワールド座標)を取得する
float GetDrawStringWidthToHandle(const WCHAR* String, const CXFont* FontHandle);

// フォントハンドルを使用して文字列を描画する
void DrawStringToHandle(float x, float y, const WCHAR* String, Color Color, const CXFont* FontHandle);
