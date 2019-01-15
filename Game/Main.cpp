#include "GameMain.h"
#include "Screen.h"
#include "ScreenManager.h"
#include "Input.h"
#include "Time.h"
#include "SystemUtils.h"

// 1フレーム
static int ProcessMessage(void)
{
	auto& time = Time::GetInstance();
	time.Update();
	time.WaitFrame();
	InputManager::GetInstance().Update();
	return !SystemUtils::running;
}

// 裏画面切り替え
static int ScreenFlip(void)
{
	static auto& context = ScreenManager::GetInstance().GetContext();

	context.Flush();

	return true;
}

// プログラムのエントリーポイント
int main(void)
{
	// 初期状態の画面モードの設定
	//ScreenManager::GetInstance().SetFontSize(SCREEN_FONT_SIZE);
	//ScreenManager::GetInstance().SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//ScreenManager::GetInstance().SetPixelSize(14);
	ScreenManager::GetInstance().SetFontSize(7);
	ScreenManager::GetInstance().SetWindowSize(640, 480);
	ScreenManager::GetInstance().SetCursorVisibility(false);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	static auto& context = ScreenManager::GetInstance().GetContext();

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
