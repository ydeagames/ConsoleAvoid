#include "GameMain.h"

// <Gameクラス>

using namespace CXLib;

// ゲームの初期化処理
Game::Game()
{
	font_pong = CreateFontToHandle(CXFontType::CXFONT_PONG, 12);
	font = CreateFontToHandle(CXFontType::CXFONT_DEFAULT, 1);
	obj_pos = { 10, 10 };
}

// ゲームの終了処理
Game::~Game()
{
}

// ゲームの更新処理
void Game::Update(void)
{
	fps.Update();

	if (InputManager::GetInstance().key->GetButton(VK_ESCAPE))
		SystemUtils::ExitGame();

	float speed = 10 * Time::deltaTime;
	if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
		obj_pos += Vector2::up * speed;
	if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
		obj_pos += Vector2::down * speed;
	if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
		obj_pos += Vector2::left * speed;
	if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
		obj_pos += Vector2::right * speed;
}

// ゲームの描画処理
void Game::Render(void)
{
	static auto& context = ScreenManager::GetInstance().GetContext();

	Vector2 boundsMax = Vector2{ context.boundsMax } * ConsoleToScreen;
	DrawStringToHandle(boundsMax - obj_pos, L"Pong Game!", Colors::Red, &font_pong);
	for (int iy = 0; iy < 2; iy++)
		for (int ix = 0; ix < 10; ix++)
			DrawCircle(obj_pos + Vector2{ ix, iy }*10, 5, Colors::Blue, true);

	DrawStringToHandle(obj_pos, L"あsu\nshiいう\nsus\nhi↑え↑お┃か", Colors::White, &font);
	DrawDashedLine(Vector2::zero, Vector2{ context.boundsMax } * ConsoleToScreen, Colors::Green, 8);

	if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
		DrawStringToHandle(Vector2{ 15, 35 }, L"↑", Colors::White, &font);
	if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
		DrawStringToHandle(Vector2{ 15, 45 }, L"↓", Colors::White, &font);
	if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
		DrawStringToHandle(Vector2{ 10, 40 }, L"←", Colors::White, &font);
	if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
		DrawStringToHandle(Vector2{ 20, 40 }, L"→", Colors::White, &font);

	auto& time = Time::GetInstance();
	auto sleeptime = std::max(std::chrono::nanoseconds::zero(), time.interval - time.delta_processing);
	ScreenManager::GetInstance().SetTitle(
		String::Format(L"FPS: %5.2f ( %2lld )",
			fps.GetFrameRate(),
			std::chrono::duration_cast<std::chrono::milliseconds>(sleeptime).count()
		)
	);
}
