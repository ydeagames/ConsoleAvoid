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
#include "Time.h"



// グローバル変数定義 ======================================================

// 終了リクエスト
static bool exit_request = false;



// 関数定義 ================================================================

// 1フレーム
static int ProcessMessage(void)
{
	auto& time = Time::GetInstance();
	time.Update();
	time.WaitFrame();
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
	//ScreenManager::GetInstance().SetFontSize(SCREEN_FONT_SIZE);
	//ScreenManager::GetInstance().SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//ScreenManager::GetInstance().SetPixelSize(14);
	ScreenManager::GetInstance().SetFontSize(7);
	ScreenManager::GetInstance().SetWindowSize(640, 480);
	ScreenManager::GetInstance().SetCursorVisibility(CURSOR_INVISIBLE);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

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
