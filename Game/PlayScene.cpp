#include "PlayScene.h"

int PlayScene::score = 0;

PlayScene::PlayScene()
{
	static constexpr auto gameAspect = Vector2{ 16, 9 };
	static constexpr float gameAspectRatio = gameAspect.x / gameAspect.y;
	static auto fieldSize = Vector2{ gameAspectRatio, 1.f };
	static auto fieldMargin = Vector2::one * .1f;

	class Player : public Component
	{
		const float speed = .1f;
		Vector2 last_pos;
		Vector2 last_click;

		void Update()
		{
			auto& rigidbody = gameObject()->GetComponent<Rigidbody>();

			rigidbody->vel = Vector2::zero;
			if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
				rigidbody->vel += Vector2::up * speed;
			if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
				rigidbody->vel += Vector2::down * speed;
			if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
				rigidbody->vel += Vector2::left * speed;
			if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
				rigidbody->vel += Vector2::right * speed;

			auto& transform = gameObject()->transform();
			auto& parentInverse = transform->GetParentMatrix().Inverse();
			auto point = InputManager::GetInstance().mouse->GetPosition() * parentInverse;
			//transform->position = point;
			if (InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
			{
				last_pos = transform->position;
				last_click = point;
			}
			if (InputManager::GetInstance().mouse->GetButton(MouseInput::MOUSE_INPUT_LEFT))
			{
				auto sub = point - last_click;
				auto newpos = last_pos + sub;
				rigidbody->vel = (newpos - transform->position) * MathUtils::Lerp(Time::deltaTime, 16, 1);
			}

			auto& collider = gameObject()->GetComponent<BoxCollider>();
			Bounds bounds = Bounds::CreateFromCenter(Vector2::zero, fieldSize).Expand(-transform->scale * collider->shape.size / 2);
			transform->position = bounds.ClosestPoint(transform->position);
		}
	};

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
	auto field = GameObject::Create("Field");
	field->AddNewComponent<Canvas>();
	field->transform()->static_object = true;

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
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/back.ppm") });
	back->transform()->static_object = true;

	auto border = GameObject::Create("Border");
	border->transform()->parent = field->transform();
	border->transform()->scale = Vector2{ gameAspectRatio, 1.f };
	border->transform()->static_object = true;
	border->AddNewComponent<BoxRenderer>()->material = Material{}.SetBorder(Colors::Red);

	class FireController : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			Bounds bounds = Bounds::CreateFromCenter(Vector2::zero, fieldSize * 2 + fieldMargin);
			if (!bounds.Contains(transform->position))
				gameObject()->Destroy();
			score += 1;
		}
	};

	class FireGenerator : public Component
	{
		float SpeedMultiply = 1.001f;
		float speed = .1f;
		float IntervalMultiply = .97f;
		float interval = 1.5f;
		float minInterval = .25f;
		Timer timer;
		Texture texture;

		void Start()
		{
			timer = Timer{}.Start(interval);
			texture = Texture{ LoadGraph("Resources/Textures/fire.ppm", Transparent::FirstColor) };
		}

		void Update()
		{
			if (timer.IsFinished())
			{
				float direction = Random::Range(0.f, MathUtils::ToRadians(360));
				auto size = fieldSize / 2 + fieldMargin;
				auto vec = (Vector2::right * size.Length()).Rotate(direction);
				auto trace = (size.y / size.x < std::abs(vec.y / vec.x)) ? vec * std::abs(size.y / vec.y) : vec * std::abs(size.x / vec.x);
				float vecdirection = Random::Range(-MathUtils::ToRadians(90), MathUtils::ToRadians(90));
				auto vecnorm = (size.y / size.x < std::abs(vec.y / vec.x)) ? (vec.y < 0 ? Vector2::up : Vector2::down) : (vec.x < 0 ? Vector2::left : Vector2::right);
				auto vel = (-vecnorm * speed).Rotate(vecdirection);

				auto fire = GameObject::Create("Fire", 2);
				fire->AddNewComponent<TextureRenderer>(texture);
				fire->transform()->parent = GameObject::Find("Field")->transform();
				fire->transform()->position = trace;
				fire->transform()->scale = Vector2::one * .1f;
				fire->AddNewComponentAs<Collider, CircleCollider>(Circle{ Vector2::zero, .5f })->isTrigger = true;
				fire->AddNewComponent<Rigidbody>()->vel = vel;
				fire->AddNewComponent<FireController>();
				fire->eventbus()->Register([fire](TriggerEnterEvent& eventobj) {
					//_RPT0(_CRT_WARN, "hit");
					SceneManager::GetInstance().RequestScene(SceneID::RESULT);
					});
				interval = std::max(minInterval, interval * IntervalMultiply);
				speed *= SpeedMultiply;
				timer.Start(interval);
			}
		}
	};

	auto firegen = GameObject::Create("FireGenerator");
	firegen->transform()->parent = field->transform();
	firegen->AddNewComponent<FireGenerator>();

	auto player = GameObject::Create("Player", 3);
	player->transform()->parent = field->transform();
	player->transform()->scale = Vector2::one * .3f;
	player->AddNewComponent<Player>();
	player->AddNewComponent<Rigidbody>(Vector2{}, std::vector<int>{ 2 });
	player->AddNewComponentAs<Collider, BoxCollider>(Box{ Vector2{ 0.f, .1f }, Vector2{ .6f, .7f } });
	auto texture = Texture{
		std::vector<CXImage>{
			LoadGraph("Resources/Textures/bomb1.ppm"),
			LoadGraph("Resources/Textures/bomb2.ppm", Transparent::FirstColor),
		},
		1
	};
	//player->AddNewComponent<BoxRenderer>()->material = Material{}.SetBorder(Colors::Blue);
	player->AddNewComponent<TextureRenderer>(texture);

	class ScorePanel : public Component
	{
		CXFont font_pong;

		void Start()
		{
			font_pong = CreateFontToHandle(CXFontType::CXFONT_PONG, 4);
		}

		void Render()
		{
			auto& transform = gameObject()->transform();
			DrawStringToHandle(String::Format(L"SCORE: %d", score), Colors::White, font_pong, Matrix3::CreateTranslation(Vector2{ 30, 30 }));
		}
	};
	auto scorepanel = GameObject::Create("ScorePanel", 5);
	scorepanel->AddNewComponent<ScorePanel>();

	class SceneHook : public Component
	{
		void Update()
		{
			if (InputManager::GetInstance().key->GetButtonDown(VK_SPACE))
				SceneManager::GetInstance().RequestScene(SceneID::PLAY);
		}
	};

	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

PlayScene::~PlayScene()
{
}
