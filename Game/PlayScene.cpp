#include "PlayScene.h"

PlayScene::PlayScene()
{
	class Player : public Component
	{
		Vector2 last_pos;
		Vector2 last_click;

		void Update()
		{
			auto& rigidbody = gameObject()->GetComponent<Rigidbody>();

			float speed = .1f;
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
				transform->position = last_pos + sub;
			}
		}

		CXFont font_pong = CreateFontToHandle(CXFontType::CXFONT_PONG, 6);

		void Render()
		{
			auto& transform = gameObject()->transform();
			DrawStringToHandle(String::Format(L"(%.2f, %.2f)", transform->position.x, transform->position.y), Colors::White, font_pong, Matrix3::CreateTranslation(Vector2{ 50, 50 }));
		}
	};

	class FireController : public Component
	{
		void Start()
		{

		}

		void Update()
		{

		}
	};

	class FireGenerator : public Component
	{
		void Start()
		{

		}

		void Update()
		{

		}
	};

	static auto gameAspect = Vector2{ 16, 9 };
	static float gameAspectRatio = gameAspect.x / gameAspect.y;

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

	auto back = GameObject::Create("Back");
	back->transform()->parent = field->transform();
	back->transform()->scale = Vector2{ gameAspectRatio, 1.f };
	back->transform()->static_object = true;
	back->AddNewComponent<BoxRenderer>()->material = Material{}.SetBase(Colors::Green).SetBorder(Colors::Red);

	auto fire = GameObject::Create("Fire", 2);
	fire->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/fire.ppm", Transparent::FirstColor) });
	fire->transform()->parent = field->transform();
	fire->transform()->position = Vector2{ 0, 0 };
	fire->transform()->scale = Vector2::one * .05f;
	fire->AddNewComponentAs<Collider, CircleCollider>(Circle{ Vector2::zero, .5f });
	fire->eventbus()->Register([fire](CollisionEnterEvent& eventobj) {
		_RPT0(_CRT_WARN, "hit");
	});

	auto player = GameObject::Create("Player", 3);
	player->transform()->parent = field->transform();
	player->transform()->scale = Vector2::one * .15f;
	player->AddNewComponent<Player>();
	player->AddNewComponent<Rigidbody>(Vector2{}, std::vector<int>{ 2 });
	player->AddNewComponentAs<Collider, BoxCollider>(Box{ Vector2::zero, Vector2{ .5f, 1.f } });
	auto texture = Texture{
		std::vector<CXImage>{
			LoadGraph("Resources/Textures/bomb1.ppm"),
			LoadGraph("Resources/Textures/bomb2.ppm", Transparent::FirstColor),
		},
		1
	};
	player->AddNewComponent<BoxRenderer>()->material = Material{}.SetBorder(Colors::Blue);
	player->AddNewComponent<TextureRenderer>(texture);

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
