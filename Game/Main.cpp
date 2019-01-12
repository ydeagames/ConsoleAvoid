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
#include "Screen.h"
#include "ScreenManager.h"
#include "Input.h"
#include "MathUtils.h"



// グローバル変数定義 ======================================================

// 最後の時刻
static std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
// 1フレームの間隔
static std::chrono::nanoseconds delta_clock;
// 1フレームの秒
float delta_time;
// 終了リクエスト
static bool exit_request = false;
// FPS
static const int FPS = 60;



// 関数定義 ================================================================

// 1フレーム
static int ProcessMessage(void)
{
	std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
	delta_clock = now - last_time;
	last_time = now;

	delta_time = delta_clock.count() * 1e-9f;

	static auto interval = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / FPS;
	auto sleeptime = interval - delta_clock;
	Sleep(static_cast<DWORD>(std::max(0LL, std::chrono::duration_cast<std::chrono::milliseconds>(sleeptime).count())));

	InputManager::GetInstance().Update();

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
	ScreenManager::GetInstance().SetFontSize(SCREEN_FONT_SIZE/2, SCREEN_FONT_SIZE);
	ScreenManager::GetInstance().SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	ScreenManager::GetInstance().SetCursorVisibility(CURSOR_INVISIBLE);

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
