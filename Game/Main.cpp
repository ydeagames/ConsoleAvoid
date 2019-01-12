//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Main.cpp
//!
//! @brief  ゲームプログラムのエントリーポイントのソースファイル
//!
//! @date   2018/07/18
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================

#include "Game.h"
#include "Console.h"
#include "GameMain.h"
#include "BufferedConsole.h"
#include "ScreenManager.h"
#include "Input.h"
#include "MathUtils.h"



// グローバル変数定義 ======================================================

// 最後の時刻
static struct timespec last_clock;
// 1フレームの秒
float delta_time;
// 終了リクエスト
static bool exit_request = false;



// 関数定義 ================================================================

// 1フレーム
static int ProcessMessage(void)
{
	struct timespec now;
	timespec_get(&now, TIME_UTC);
	delta_time = std::min(60.f, ((now.tv_sec - last_clock.tv_sec) + (now.tv_nsec - last_clock.tv_nsec)/1000.f/1000.f/1000.f) * 16);
	last_clock = now;

	InputManager::GetInstance().Update();

	Sleep(1000 / 100);

	return exit_request;
}

// 裏画面切り替え
static int ScreenFlip(void)
{
	auto& context = ScreenManager::GetInstance().GetContext();

	context.Flush();

	return true;
}

// 終了リクエスト
void ExitGame(void)
{
	exit_request = true;
}

//----------------------------------------------------------------------
//! @brief プログラムのエントリーポイント
//!
//! @retval  0 正常終了
//! @retval -1 異常終了
//----------------------------------------------------------------------
int main(void)
{
	// 初期状態の画面モードの設定
	SetFontSize(SCREEN_FONT_SIZE);
	SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetCursorVisibility(CURSOR_INVISIBLE);

	auto& context = ScreenManager::GetInstance().GetContext();

	/*
	// スピードテスト (デバッグ)
	{
		TestSpeed();
		return 0;
	}
	/**/

	// ゲームの処理
	InitializeGame();			// ゲームの初期化処理

	while (!ProcessMessage())
	{
		UpdateGame();			// ゲームの更新処理
		RenderGame();			// ゲームの描画処理

		ScreenFlip();			// 裏画面の内容を表画面に反映
		context.Clear();		// 裏画面の消去
	}

	FinalizeGame();				// ゲームの終了処理

	return 0;					// 正常終了
}
