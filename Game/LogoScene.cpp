#include "LogoScene.h"

// ロゴシーン
LogoScene::LogoScene()
{
	// 背景画像コンポーネント
	class Back : public Component
	{
		void Update()
		{
			// 画面サイズいっぱいにする
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
	// 背景画像オブジェクト
	auto& back = GameObject::Create("Back");
	back->AddNewComponent<Back>();														// 背景画像コンポーネント
	back->AddNewComponent<BoxRenderer>()->material = Material{}.SetBase(Colors::White);	// 白く塗りつぶす
	back->transform()->static_object = true;											// 静的描画

	// ロゴコンポーネント
	class Logo : public Component
	{
		// 時間
		float time = 0;

		void Update()
		{
			// 時間
			time += .01f;
			if (time > 1.f)
				// 時間経過でタイトルシーンへ
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);

			// 縦横比を維持しながら徐々に大きくしていく
			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();		// ロゴテクスチャの大きさ取得
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));			// 1:1のアスペクト比で画像の枠に収まるサイズ
			transform->scale = size * Easings::OUTCUBIC.GetEasingValueRange(std::min(1.f, time), .5f, .75f);	// OutCubicイージングを使用して画像を拡大
			transform->position = windowsize / 2;
		}
	};
	// ロゴオブジェクト
	auto& logo = GameObject::Create("Logo");
	logo->AddNewComponent<Logo>();																				// ロゴコンポーネント
	logo->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/ydeagames.ppm") });			// ロゴテクスチャ描画

	// シーンフックコンポーネント
	class SceneHook : public Component
	{
		void Update()
		{
			// スペースキーかマウス左ボタンが押されたら
			if (InputManager::GetInstance().key->GetButtonDown(VK_SPACE)
				|| InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
				// タイトルシーンへ移動
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);
		}
	};
	// シーンフックオブジェクト
	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

LogoScene::~LogoScene()
{
}
