#include "PlayScene.h"

PlayScene::PlayScene()
{
	class Player : public Component
	{
		CXImage textures[2];
		Vector2 last_pos;
		Vector2 last_click;

		void Start()
		{
			textures[0] = LoadGraph("Resources/Textures/bomb1.ppm", Transparent::FirstColor);
			textures[1] = LoadGraph("Resources/Textures/bomb2.ppm", Transparent::FirstColor);
		}

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
			transform->position = InputManager::GetInstance().mouse->GetPosition() * parentInverse;
			//if (InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
			//{
			//	last_pos = transform->position;
			//	last_click = InputManager::GetInstance().mouse->GetPosition();
			//}
			//if (InputManager::GetInstance().mouse->GetButton(MouseInput::MOUSE_INPUT_LEFT))
			//{
			//	auto sub = InputManager::GetInstance().mouse->GetPosition() - last_click;
			//	transform->position = last_pos + sub;
			//}
		}

		void Render()
		{
			auto& transform = gameObject()->transform();

			auto& texture = textures[static_cast<int>(Time::time) % 2];

			texture.DrawGraph(Matrix3::CreateTranslation(-Vector2::one / 2) * transform->GetMatrix());
		}
	};

	auto field = GameObject::Create("Field");
	field->transform()->position = GetWindowSize() / 2;
	field->transform()->scale = Vector2::one * 10;

	auto player = GameObject::Create("Player");
	player->transform()->parent = field->transform();
	player->AddNewComponent<Player>();
	player->AddNewComponent<Rigidbody>();

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
