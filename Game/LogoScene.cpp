#include "LogoScene.h"

LogoScene::LogoScene()
{
	class Logo : public Component
	{
		CXImage image;
		float time = 0;

		void Start()
		{
			image = LoadGraph("Resources/Textures/ydeagames.ppm");
		}

		void Update()
		{
			time += .01f;
			if (time > 1.f
				|| InputManager::GetInstance().key->GetButtonDown(VK_SPACE)
				|| InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);
		}

		void Render()
		{
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Inner(image.GetSize(), windowsize);
			DrawBox(Vector2::zero, windowsize, Colors::White, true);
			image.DrawGraph(Matrix3::CreateScale(size)
				* Matrix3::CreateScale(Vector2::one * Easings::OUTCUBIC.GetEasingValueRange(std::min(1.f, time), .5f, .75f))
				* Matrix3::CreateTranslation(windowsize / 2));
		}
	};

	GameObject::Create("Logo")->AddNewComponent<Logo>();
}

LogoScene::~LogoScene()
{
}
