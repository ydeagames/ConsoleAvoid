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

			float speed = 100;
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
	};

	auto gameAspect = Vector2{ 16, 9 };
	auto windowAspect = GetWindowSize();
	auto windowAspectRatio = windowAspect.x / windowAspect.y;

	auto field = GameObject::Create("Field");
	field->transform()->position = GetWindowSize() / 2;
	field->transform()->scale = AspectUtils::Height(Vector2::one, GetWindowSize());

	auto back = GameObject::Create("Back");
	back->transform()->parent = field->transform();
	back->transform()->scale = Vector2{ windowAspectRatio, 1.f };
	back->AddNewComponent<BoxRenderer>()->material = Material{}.SetBase(Colors::Green).SetBorder(Colors::Red);

	auto player = GameObject::Create("Player");
	player->transform()->parent = field->transform();
	player->transform()->scale = Vector2::one * .1f;
	player->AddNewComponent<Player>();
	player->AddNewComponent<Rigidbody>();
	auto texture = Texture{
		std::vector<CXImage>{
			LoadGraph("Resources/Textures/bomb1.ppm", Transparent::FirstColor),
			LoadGraph("Resources/Textures/bomb2.ppm", Transparent::FirstColor),
		},
		1
	};
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
