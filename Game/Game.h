#pragma once

// 定数の定義 ==============================================================

// <システム> ----------------------------------------------------------

#define GAME_TITLE "Sample Game"																			// ゲームタイトル


// <画面> --------------------------------------------------------------

#define DEFAULT_SCREEN_WIDTH		(120)																	// デフォルトの画面の高さ[em]
#define DEFAULT_SCREEN_HEIGHT		(30)																	// デフォルトの画面の幅[em]
#define DEFAULT_SCREEN_FONT_SIZE	(16)																	// デフォルトの文字の大きさ[pt]

#define SCREEN_RESOLUTION_X			(7.f / 4.f)																// 縦横比 (横)
#define SCREEN_RESOLUTION_Y			(1.f)																	// 縦横比 (縦)
#define SCREEN_FONT_SIZE			(7)																		// 文字の大きさ[pt]

#define SCREEN_WIDTH				(DEFAULT_SCREEN_WIDTH * DEFAULT_SCREEN_FONT_SIZE / SCREEN_FONT_SIZE)	// 画面の幅[em]
#define SCREEN_HEIGHT				(DEFAULT_SCREEN_HEIGHT * DEFAULT_SCREEN_FONT_SIZE / SCREEN_FONT_SIZE)	// 画面の高さ[em]

#define SCREEN_TOP					(0)																		// 画面の上端
#define SCREEN_BOTTOM				(SCREEN_HEIGHT - SCREEN_TOP)											// 画面の下端
#define SCREEN_LEFT					(0)																		// 画面の左端
#define SCREEN_RIGHT				(SCREEN_WIDTH - SCREEN_LEFT)											// 画面の右端

#define SCREEN_CENTER_X				(SCREEN_WIDTH  / 2)														// 画面の中央(X座標)
#define SCREEN_CENTER_Y				(SCREEN_HEIGHT / 2)														// 画面の中央(Y座標)

#define WORLD_WIDTH					(SCREEN_WIDTH / SCREEN_RESOLUTION_X)
#define WORLD_HEIGHT				(SCREEN_HEIGHT / SCREEN_RESOLUTION_Y)

#define WORLD_TOP					(SCREEN_TOP / SCREEN_RESOLUTION_Y)										// 画面の上端
#define WORLD_BOTTOM				(SCREEN_BOTTOM / SCREEN_RESOLUTION_Y)									// 画面の下端
#define WORLD_LEFT					(SCREEN_LEFT / SCREEN_RESOLUTION_X)										// 画面の左端
#define WORLD_RIGHT					(SCREEN_RIGHT / SCREEN_RESOLUTION_X)									// 画面の右端

#define WORLD_CENTER_X				(SCREEN_CENTER_X / SCREEN_RESOLUTION_X)									// 画面の中央(X座標)
#define WORLD_CENTER_Y				(SCREEN_CENTER_Y / SCREEN_RESOLUTION_Y)									// 画面の中央(Y座標)




// グローバル変数の宣言 ====================================================

extern float delta_time;																					// 前のフレームからの経過時間(秒)




// 関数の宣言 ==============================================================

// 終了リクエスト
void ExitGame(void);
