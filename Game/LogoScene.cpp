#include "LogoScene.h"

LogoScene::LogoScene()
{
	class Back : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = windowsize;
			transform->position = windowsize / 2;

			//static auto& context = ScreenManager::GetInstance().GetOutputContext();
			//static auto gameAspect = Vector2{ 240, 151 };
			//auto size = AspectUtils::Outer(gameAspect, windowsize);
			//int fontsize = static_cast<int>(std::round(size.y / gameAspect.y * 7 / 3)) * 3;
			//if (context.fontSize != fontsize)
			//	ScreenManager::GetInstance().SetPixelSize(fontsize);
		}
	};

	auto& back = GameObject::Create("Back");
	back->AddNewComponent<Back>();
	back->AddNewComponent<BoxRenderer>()->material = Material{}.SetBase(Colors::White);
	back->transform()->static_object = true;

	class Logo : public Component
	{
		float time = 0;

		void Update()
		{
			time += .01f;
			if (time > 1.f)
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);

			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));
			transform->scale = size * Easings::OUTCUBIC.GetEasingValueRange(std::min(1.f, time), .5f, .75f);
			transform->position = windowsize / 2;
		}
	};

	auto& logo = GameObject::Create("Logo");
	logo->AddNewComponent<Logo>();
	logo->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/ydeagames.ppm") });

	class SceneHook : public Component
	{
		void Update()
		{
			if (InputManager::GetInstance().key->GetButtonDown(VK_SPACE)
				|| InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);
		}
	};

	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

LogoScene::~LogoScene()
{
}
