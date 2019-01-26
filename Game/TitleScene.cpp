#include "TitleScene.h"

// タイトルシーン
TitleScene::TitleScene()
{
	// 背景画像コンポーネント
	class Back : public Component
	{
		void Update()
		{
			// 画面サイズいっぱいにする
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = AspectUtils::Outer(Vector2::one, windowsize);
			transform->position = windowsize / 2;
		}
	};
	// 背景画像オブジェクト
	auto& back = GameObject::Create("Back");
	back->AddNewComponent<Back>();																	// 背景画像コンポーネント
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/titlebg.ppm") });	// タイトル画像で塗りつぶす
	back->transform()->static_object = true;														// 静的描画

	// タイトルUIコンポーネント
	class Title : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();	// リザルトUIテクスチャの大きさ取得
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));		// 1:1のアスペクト比で画像の枠に収まるサイズ
			transform->scale = size;																		// 最大化
			transform->position = windowsize / 2;
		}
	};
	// タイトルUIオブジェクト
	auto& title = GameObject::Create("Title");
	title->AddNewComponent<Title>();																						  // リザルトUIコンポーネント
	title->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/title.ppm", Transparent::FirstColor) });	  // リザルトUIテクスチャ描画
	title->transform()->static_object = true;

	// クリックアイコンコンポーネント
	class Click : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = Vector2::one * 20;
			transform->position = Vector2{ 10.f, windowsize.y - 10.f };	 // 左下
		}
	};
	// クリックアイコンオブジェクト
	auto& click = GameObject::Create("Click");
	click->AddNewComponent<Click>();
	click->AddNewComponent<TextureRenderer>(Texture{
		std::vector<CXImage>{
			LoadGraph("Resources/Textures/click1.ppm", Transparent::FirstColor),
			LoadGraph("Resources/Textures/click2.ppm", Transparent::FirstColor)
		}, .5f
		});		// クリックアニメーション

	// シーンフックコンポーネント
	class SceneHook : public Component
	{
		void Update()
		{
			// スペースキーかマウス左ボタンが押されたら
			if (InputManager::GetInstance().key->GetButtonDown(VK_SPACE)
				|| InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
				// タイトルシーンへ移動
				SceneManager::GetInstance().RequestScene(SceneID::PLAY);
		}
	};
	// シーンフックオブジェクト
	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

TitleScene::~TitleScene()
{
}
