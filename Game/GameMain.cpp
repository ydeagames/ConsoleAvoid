#include "GameMain.h"
#include "MathUtils.h"
#include "Input.h"
#include "CXLib.h"
#include "CXFont.h"
#include "Vector2.h"
#include "ScreenManager.h"
#include "Screen.h"
#include "Time.h"
#include "FrameTimer.h"
#include "String.h"
#include "SystemUtils.h"

// 定数の定義 ==============================================================


// グローバル変数の定義 ====================================================

using namespace CXLib;

// <フォント> ----------------------------------------------------------
CXFont g_font_pong;
CXFont g_font;

// <ポーズ> ------------------------------------------------------------
bool g_paused;
int g_pause_select;

Vector2 g_pos;
FrameTimer fps;

// 関数の定義 ==============================================================

//----------------------------------------------------------------------
//! @brief ゲームの初期化処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void InitializeGame(void)
{
	g_font_pong = CreateFontToHandle(CXFontType::CXFONT_PONG, 12);
	g_font = CreateFontToHandle(CXFontType::CXFONT_DEFAULT, 1);
	g_pos = { 10, 10 };
}

//----------------------------------------------------------------------
//! @brief ゲームの更新処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void UpdateGame(void)
{
	fps.Update();

	if (InputManager::GetInstance().key->GetButton(VK_ESCAPE))
		SystemUtils::ExitGame();

	float speed = 10 * Time::deltaTime;
	if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
		g_pos += Vector2::up * speed;
	if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
		g_pos += Vector2::down * speed;
	if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
		g_pos += Vector2::left * speed;
	if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
		g_pos += Vector2::right * speed;
}

//----------------------------------------------------------------------
//! @brief ゲームの描画処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void RenderGame(void)
{
	static auto& context = ScreenManager::GetInstance().GetContext();

	Vector2 boundsMax = Vector2{ context.boundsMax } * ConsoleToScreen;
	DrawStringToHandle(boundsMax - g_pos, L"Pong Game!", Colors::Red, &g_font_pong);
	for (int iy = 0; iy < 2; iy++)
		for (int ix = 0; ix < 10; ix++)
			DrawCircle(g_pos + Vector2{ ix, iy }*10, 5, Colors::Blue, true);
	DrawDashedLine(Vector2::zero, Vector2{ context.boundsMax } * ConsoleToScreen, Colors::Green, 8);
	//DrawLine(Vector2::zero, Vector2{ context.boundsMax } * ConsoleToScreen, Colors::Green);

	//POINT point;
	//GetCursorPos(&point);
	//std::string str = "x=" + std::to_string(static_cast<int>(point.x)) + ", " + "y=" + std::to_string(static_cast<int>(point.y));
	//DrawStringToHandle(10, 25, str.c_str(), ATTR_WHITE, &g_font);

	//DrawBox(5, 5, 95, 95, ATTR_WHITE, false);

	DrawStringToHandle(g_pos, L"あsu\nshiいう\nsus\nhi↑え↑お┃か", Colors::White, &g_font);

	if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
		DrawStringToHandle(Vector2{ 15, 35 }, L"↑", Colors::White, &g_font);
	if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
		DrawStringToHandle(Vector2{ 15, 45 }, L"↓", Colors::White, &g_font);
	if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
		DrawStringToHandle(Vector2{ 10, 40 }, L"←", Colors::White, &g_font);
	if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
		DrawStringToHandle(Vector2{ 20, 40 }, L"→", Colors::White, &g_font);

	//DrawBox(10, 10, 90, 90, ATTR_WHITE, false);

	auto& time = Time::GetInstance();
	auto sleeptime = std::max(std::chrono::nanoseconds::zero(), time.interval - time.delta_processing);
	ScreenManager::GetInstance().SetTitle(
		String::Format(L"FPS: %5.2f ( %2lld )",
			fps.GetFrameRate(),
			std::chrono::duration_cast<std::chrono::milliseconds>(sleeptime).count()
		)
	);
}

//----------------------------------------------------------------------
//! @brief ゲームの終了処理
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FinalizeGame(void)
{
}
