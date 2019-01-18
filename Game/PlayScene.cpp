#include "PlayScene.h"

PlayScene::PlayScene()
{
	class Player : public Component
	{
		CXImage textures[2];

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
		}

		void Render()
		{
			
		}
	};

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
