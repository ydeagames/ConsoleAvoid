#include "GameMain.h"
#include "LogoScene.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "ResultScene.h"

// <Game�N���X>

using namespace CXLib;

// �Q�[���̏���������
Game::Game()
{
	SceneManager& scene_manager = SceneManager::GetInstance();

	scene_manager.AddScene<LogoScene>(SceneID::LOGO);
	scene_manager.AddScene<TitleScene>(SceneID::TITLE);
	scene_manager.AddScene<PlayScene>(SceneID::PLAY);
	scene_manager.AddScene<ResultScene>(SceneID::RESULT);

	scene_manager.SetStartScene(SceneID::LOGO);
}

// �Q�[���̏I������
Game::~Game()
{
}

// �Q�[���̍X�V����
void Game::Update(void)
{
	m_frame_timer.Update();
	Timer::Tick();

	if (InputManager::GetInstance().key->GetButton(VK_ESCAPE))
		SystemUtils::ExitGame();

	InputManager::GetInstance().Update();

	SceneManager::GetInstance().UpdateActiveScene();
}

// �Q�[���̕`�揈��
void Game::Render(void)
{
	SceneManager::GetInstance().RenderActiveScene();

	{
		auto& time = Time::GetInstance();
		auto sleeptime = std::max(std::chrono::nanoseconds::zero(), time.interval - time.delta_processing);
		ScreenManager::GetInstance().SetTitle(
			String::Format(L"���₤���I�{���ւ� - FPS: %5.2f ( Sleep: %2lldms )",
				m_frame_timer.GetFrameRate(),
				std::chrono::duration_cast<std::chrono::milliseconds>(sleeptime).count()
			)
		);
	}
}
