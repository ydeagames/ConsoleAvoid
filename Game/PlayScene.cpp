#include "PlayScene.h"

float PlayScene::score = 0;

PlayScene::PlayScene()
{
	// ゲームの想定サイズ
	static constexpr auto gameAspect = Vector2{ 16, 9 };
	// ゲームの想定縦横比
	static constexpr float gameAspectRatio = gameAspect.x / gameAspect.y;
	// フィールドサイズ
	static auto fieldSize = Vector2{ gameAspectRatio, 1.f };
	// フィールドサイズのマージン
	static auto fieldMargin = Vector2::one * .1f;

	// スコアを初期化
	score = 0;

	// プレイヤーコンポーネント
	class Player : public Component
	{
		const float speed = .4f;	// キー操作でのスピード
		Vector2 last_pos;			// 最後の位置
		Vector2 last_click;			// 最後のクリック座標
		bool dragged;				// ドラッグされているか

		bool lastBombed;			// 前フレームは爆ぜていたが
		bool bombed;				// 爆ぜたか
		Timer bombedTimer;			// 爆ぜてからの時間

		void Start()
		{
			dragged = false;
			bombed = lastBombed = false;

			bool& bombedref = bombed;
			// イベントバスに 火の玉に触れたときのイベント を登録
			gameObject()->eventbus()->Register([&bombedref](TriggerEnterEvent& eventobj) {
				// イベントが発火したときの処理
				//_RPT0(_CRT_WARN, "hit");
				// 爆発したフラグをON
				bombedref = true;
			});
		}

		void Update()
		{
			// 爆発したら
			if (bombed)
			{
				// 爆発直後ならば
				if (!lastBombed)
				{
					// 爆発後のタイマーをスタート
					bombedTimer = Timer{}.Start(2.f);
					// 爆発アニメーション
					gameObject()->GetComponent<TextureRenderer>()->texture = Texture{ std::vector<CXImage>{
							LoadGraph("Resources/Textures/bomb_explosion1.ppm", Transparent::FirstColor),
							LoadGraph("Resources/Textures/bomb_explosion2.ppm", Transparent::FirstColor),
							LoadGraph("Resources/Textures/bomb_explosion3.ppm", Transparent::FirstColor),
					},
					2.f / 3,
					false
					};
					// Rigidbodyの速度を0にする
					gameObject()->GetComponent<Rigidbody>()->vel = Vector2::zero;
					lastBombed = true;
				}
				// 爆ぜてから時間が建ったら
				if (bombedTimer.IsFinished())
					// リザルトシーンへ
					SceneManager::GetInstance().RequestScene(SceneID::RESULT);
			}
			// 一時停止していなかったら
			else if (Time::speed > 0)
			{
				auto& rigidbody = gameObject()->GetComponent<Rigidbody>();

				// キーを押してRigidbodyの速度を追加
				rigidbody->vel = Vector2::zero;
				if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
					rigidbody->vel += Vector2::up * speed;
				if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
					rigidbody->vel += Vector2::down * speed;
				if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
					rigidbody->vel += Vector2::left * speed;
				if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
					rigidbody->vel += Vector2::right * speed;

				// カーソルのワールド座標を取得
				auto& transform = gameObject()->transform();
				auto& parentInverse = transform->GetParentMatrix().Inverse();
				auto point = InputManager::GetInstance().mouse->GetPosition() * parentInverse;
				// 左クリックされたらドラッグ可能
				if (InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
				{
					last_pos = transform->position;
					last_click = point;
					dragged = true;
				}
				if (dragged)
				{
					auto sub = point - last_click;
					auto newpos = last_pos + sub;
					rigidbody->vel = -sub;
					transform->position = newpos;
				}
				if (InputManager::GetInstance().mouse->GetButtonUp(MouseInput::MOUSE_INPUT_LEFT))
				{
					dragged = false;
				}

				// 範囲外に移動しないようにする
				auto& collider = gameObject()->GetComponent<BoxCollider>();
				Bounds bounds = Bounds::CreateFromCenter(Vector2::zero, fieldSize).Expand(-transform->scale * collider->shape.size / 2);
				transform->position = bounds.ClosestPoint(transform->position);
			}
		}
	};

	// フィールドコンポーネント(子オブジェクトはすべてフィールド座標に変換される)
	class Canvas : public Component
	{
		void Update()
		{
			auto windowsize = GetWindowSize();
			auto& transform = gameObject()->transform();
			transform->position = windowsize / 2;
			transform->scale = AspectUtils::Height(Vector2::one, AspectUtils::Inner(gameAspect, windowsize));
		}
	};
	// フィールドオブジェクト(子オブジェクトはすべてフィールド座標に変換される)
	auto field = GameObject::Create("Field");
	field->AddNewComponent<Canvas>();
	field->transform()->static_object = true;	// 静的オブジェクト

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
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/back.ppm") });	// リザルト画面で塗りつぶす
	back->transform()->static_object = true;														// 静的描画

	//// フィールドの境界線
	//auto border = GameObject::Create("Border");
	//border->transform()->parent = field->transform();
	//border->transform()->scale = Vector2{ gameAspectRatio, 1.f };
	//border->transform()->static_object = true;
	//border->AddNewComponent<BoxRenderer>()->material = Material{}.SetBorder(Colors::Red);

	// 火の玉コントローラーコンポーネント
	class FireController : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			Bounds bounds = Bounds::CreateFromCenter(Vector2::zero, fieldSize * 2 + fieldMargin);
			if (!bounds.Contains(transform->position))
				gameObject()->Destroy();
			score += Time::deltaTime * 20;
		}
	};

	// 火の玉ジェネレーターコンポーネント
	class FireGenerator : public Component
	{
	public:
		// ゲームが始まっているか
		bool started;

	private:
		float SpeedMultiply = 1.001f;	// 1弾発射ごとのスピード加速率
		float speed = .1f;				// 弾のスピード
		float IntervalMultiply = .97f;	// 1弾発射ごとの発射間隔加速率
		float interval = 1.5f;			// 弾の発射間隔
		float minInterval = .25f;		// 弾の最小発射間隔
		Timer timer;					// 弾のタイマー
		Texture texture;				// 弾のテクスチャ

		void Start()
		{
			started = false;
			timer = Timer{}.Start(interval);
			texture = Texture{ LoadGraph("Resources/Textures/fire.ppm", Transparent::FirstColor) };		// 火の玉のテクスチャ
		}

		void Update()
		{
			// ゲームが始まっていて一定間隔ごとに
			if (started && timer.IsFinished())
			{
				// 方向は内側で画面端から
				float direction = Random::Range(0.f, MathUtils::ToRadians(360));
				auto size = fieldSize / 2 + fieldMargin;
				auto vec = (Vector2::right * size.Length()).Rotate(direction);
				auto trace = (size.y / size.x < std::abs(vec.y / vec.x)) ? vec * std::abs(size.y / vec.y) : vec * std::abs(size.x / vec.x);
				float vecdirection = Random::Range(-MathUtils::ToRadians(90), MathUtils::ToRadians(90));
				auto vecnorm = (size.y / size.x < std::abs(vec.y / vec.x)) ? (vec.y < 0 ? Vector2::up : Vector2::down) : (vec.x < 0 ? Vector2::left : Vector2::right);
				auto vel = (-vecnorm * speed).Rotate(vecdirection);

				// 火の玉オブジェクト
				auto fire = GameObject::Create("Fire", 2);
				fire->AddNewComponent<TextureRenderer>(texture);													// テクスチャ
				fire->transform()->parent = GameObject::Find("Field")->transform();									// 親オブジェクト
				fire->transform()->position = trace;																// 画面端
				fire->transform()->scale = Vector2::one * .1f;														// サイズ
				fire->AddNewComponentAs<Collider, CircleCollider>(Circle{ Vector2::zero, .5f })->isTrigger = true;	// コライダー
				fire->AddNewComponent<Rigidbody>()->vel = vel;														// Rigidbody
				fire->AddNewComponent<FireController>();															// コントローラー

				interval = std::max(minInterval, interval * IntervalMultiply);										// インターバルを短く
				speed *= SpeedMultiply;																				// スピードを加速
				timer.Start(interval);																				// タイマーリセット
			}
		}
	};
	// 火の玉ジェネレーターオブジェクト
	auto firegen = GameObject::Create("FireGenerator");
	firegen->transform()->parent = field->transform();
	firegen->AddNewComponent<FireGenerator>();

	// プレイヤーオブジェクト
	auto player = GameObject::Create("Player", 3);
	player->transform()->parent = field->transform();													  // 親オブジェクト
	player->transform()->scale = Vector2::one * .3f;													  // サイズ
	player->transform()->position = Vector2{ 0.f, .3f };												  // 位置は中心から少し下
	player->AddNewComponent<Player>();																	  // プレイヤー
	player->AddNewComponent<Rigidbody>(Vector2{}, std::vector<int>{ 2 });								  // レイヤー2(火の玉)と衝突するRigidbody
	player->AddNewComponentAs<Collider, BoxCollider>(Box{ Vector2{ 0.f, .1f }, Vector2{ .4f, .5f } });	  //　コライダー
	auto texture = Texture{
		std::vector<CXImage>{
			LoadGraph("Resources/Textures/bomb1.ppm"),
			LoadGraph("Resources/Textures/bomb2.ppm", Transparent::FirstColor),
		},
		1
	};		// テクスチャ
	player->AddNewComponent<TextureRenderer>(texture);
	//player->AddNewComponent<BoxRenderer>()->material = Material{}.SetBorder(Colors::Blue);

	// スコアパネルコンポーネント
	class ScorePanel : public Component
	{
		void Update()
		{
			// スコア更新
			std::wstring str = String::Format(L"SCORE: %.0f", score);		// 書式指定子でスコア文字列生成
			gameObject()->GetComponent<FontTextRenderer>()->text = str;
		}
	};
	// スコアパネルオブジェクト
	auto scorepanel = GameObject::Create("ScorePanel", 5);
	scorepanel->AddNewComponent<ScorePanel>();
	scorepanel->transform()->parent = field->transform();														  // 親オブジェクト
	scorepanel->transform()->position = Vector2{ -gameAspectRatio / 2 + .1f, -.5f + .2f };						  // 左上
	scorepanel->transform()->scale = Vector2{ .0125f, .0125f };													  // サイズ
	scorepanel->AddNewComponent<FontTextRenderer>(CreateFontToHandle(CXFontType::CXFONT_PONG, 4), L"SCORE");	  // フォント

	// カウントダウンコンポーネント
	class Countdown : public Component
	{
		Timer timer;					  // 3秒カウンター

		void Start()
		{
			// タイマースタート
			timer = Timer{}.Start(3);
		}

		void Update()
		{
			// 終了していたら
			if (timer.IsFinished())
			{
				// ファイアジェネレーターを開始
				GameObject::Find("FireGenerator")->GetComponent<FireGenerator>()->started = true;
				// オブジェクトを消去
				gameObject()->Destroy();
			}
		}
	};
	// カウントダウンオブジェクト
	auto countdown = GameObject::Create("Countdown", 5);
	countdown->AddNewComponent<Countdown>();
	countdown->transform()->parent = field->transform();
	countdown->transform()->position = Vector2::zero;
	countdown->transform()->scale = Vector2{ .2f, .2f };
	// カウントダウンアニメーション
	countdown->AddNewComponent<TextureRenderer>(Texture{ std::vector<CXImage>{
			LoadGraph("Resources/Textures/countdown3.ppm", Transparent::FirstColor),
			LoadGraph("Resources/Textures/countdown2.ppm", Transparent::FirstColor),
			LoadGraph("Resources/Textures/countdown1.ppm", Transparent::FirstColor),
	},
	1,
	false
	});

	// ポーズコンポーネント
	class Pause : public Component
	{
		// ポーズされている
		bool paused;

		void Start()
		{
			paused = false;
		}

		void Update()
		{
			// 始まっていて、スペースキーでポーズ
			if (InputManager::GetInstance().key->GetButtonDown(VK_SPACE) && GameObject::Find("FireGenerator")->GetComponent<FireGenerator>()->started)
			{
				paused = !paused;
				Time::speed = paused ? 0.f : 1.f;
				gameObject()->transform()->scale = Vector2::one * 1.f * (paused ? 1.f : 0.f);
			}
		}

		void Render()
		{
			if (paused)
			{
				// 背景を半透明
				auto& transform = gameObject()->transform();
				auto windowsize = GetWindowSize();
				DrawBox(Vector2::zero, windowsize, Colors::Black, true, Matrix3::Identity, .5f);
			}
		}
	};
	// ポーズオブジェクト
	auto pause = GameObject::Create("Pause", 8);
	pause->transform()->parent = field->transform();
	pause->transform()->position = Vector2::zero;
	pause->transform()->scale = Vector2::zero;
	pause->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/pause.ppm", Transparent::FirstColor) });
	pause->AddNewComponent<Pause>();
}

PlayScene::~PlayScene()
{
}
