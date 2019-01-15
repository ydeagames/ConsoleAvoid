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

// プログラムのエントリーポイント
int main(void)
{
	static auto& context = ScreenManager::GetInstance().GetContext();

	// 初期状態の画面モードの設定

	ScreenManager::GetInstance().SetFontSize(7);
	ScreenManager::GetInstance().SetWindowSize(640, 480);
	ScreenManager::GetInstance().SetCursorVisibility(false);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	// ゲームの処理

	// ゲームの初期化処理
	Game* game = new Game;

	while (!ProcessMessage())
	{
		// ゲームの更新処理
		game->Update();
		// ゲームの描画処理
		game->Render();

		// 裏画面の内容を表画面に反映
		context.Flush();
		// 裏画面の消去
		context.Clear();
	}

	// ゲームの終了処理
	delete game;

	return 0;
}
