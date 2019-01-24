#include "TitleScene.h"

// <ƒV[ƒ“>

TitleScene::TitleScene()
{
	class Back : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = AspectUtils::Outer(Vector2::one, windowsize);
			transform->position = windowsize / 2;
		}
	};

	auto& back = GameObject::Create("Back");
	back->AddNewComponent<Back>();
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/titlebg.ppm") });
	back->transform()->static_object = true;

	class Title : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));
			transform->scale = size;
			transform->position = windowsize / 2;
		}
	};

	auto& logo = GameObject::Create("Logo");
	logo->AddNewComponent<Title>();
	logo->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/title.ppm", Transparent::FirstColor) });
	logo->transform()->static_object = true;

	class Click : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = Vector2::one * 20;
			transform->position = Vector2{ 10.f, windowsize.y - 10.f };
		}
	};

	auto& click = GameObject::Create("Click");
	click->AddNewComponent<Click>();
	click->AddNewComponent<TextureRenderer>(Texture{ std::vector<CXImage>{ LoadGraph("Resources/Textures/click1.ppm", Transparent::FirstColor), LoadGraph("Resources/Textures/click2.ppm", Transparent::FirstColor) }, .5f });

	class SceneHook : public Component
	{
		void Update()
		{
			if (InputManager::GetInstance().key->GetButtonDown(VK_SPACE)
				|| InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
				SceneManager::GetInstance().RequestScene(SceneID::PLAY);
		}
	};

	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

TitleScene::~TitleScene()
{
}
