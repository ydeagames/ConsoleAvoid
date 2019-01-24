#include "ResultScene.h"
#include "PlayScene.h"

// <ƒV[ƒ“>

ResultScene::ResultScene()
{
	static constexpr auto gameAspect = Vector2{ 16, 9 };
	static constexpr float gameAspectRatio = gameAspect.x / gameAspect.y;

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
	auto& title = GameObject::Create("Logo");
	title->AddNewComponent<Title>();
	title->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/result.ppm", Transparent::FirstColor) });
	title->transform()->static_object = true;

	auto scorepanel = GameObject::Create("ScorePanel", 5);
	scorepanel->transform()->parent = back->transform();
	scorepanel->transform()->position = Vector2{ -.2f, .1f };
	scorepanel->transform()->scale = Vector2{ .005f, .005f };
	scorepanel->AddNewComponent<FontTextRenderer>(CreateFontToHandle(CXFontType::CXFONT_PONG, 4), String::Format(L"SCORE: %d", PlayScene::score));

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
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);
		}
	};

	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

ResultScene::~ResultScene()
{

}
