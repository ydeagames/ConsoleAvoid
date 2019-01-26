#include "ResultScene.h"
#include "PlayScene.h"

// リザルトシーン
ResultScene::ResultScene()
{
	// ゲームの想定サイズ
	static constexpr auto gameAspect = Vector2{ 16, 9 };
	// ゲームの想定縦横比
	static constexpr float gameAspectRatio = gameAspect.x / gameAspect.y;

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
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/titlebg.ppm") });	// リザルト画面で塗りつぶす
	back->transform()->static_object = true;														// 静的描画

	// リザルトUIコンポーネント																		
	class Result : public Component
	{
		void Update()
		{
			// 縦横比を維持しながら収まる範囲最大表示
			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();	// リザルトUIテクスチャの大きさ取得
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));		// 1:1のアスペクト比で画像の枠に収まるサイズ
			transform->scale = size;																		// 最大化
			transform->position = windowsize / 2;
		}
	};
	// リザルトUIオブジェクト
	auto& result = GameObject::Create("Result");
	result->AddNewComponent<Result>();																							// リザルトUIコンポーネント
	result->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/result.ppm", Transparent::FirstColor) });	// リザルトUIテクスチャ描画
	result->transform()->static_object = true;

	// スコアオブジェクト
	auto scorepanel = GameObject::Create("ScorePanel", 5);
	scorepanel->transform()->parent = back->transform();			// 親オブジェクト
	scorepanel->transform()->position = Vector2{ -.2f, .1f };		// ローカル座標
	scorepanel->transform()->scale = Vector2{ .005f, .005f };		// ローカルスケール
	scorepanel->AddNewComponent<FontTextRenderer>(CreateFontToHandle(CXFontType::CXFONT_PONG, 4), String::Format(L"SCORE: %.0f", PlayScene::score));

	// クリックアイコンコンポーネント
	class Click : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = Vector2::one * 20;
			transform->position = Vector2{ 10.f, windowsize.y - 10.f };	// 左下
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
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);
		}
	};
	// シーンフックオブジェクト
	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

ResultScene::~ResultScene()
{
}
