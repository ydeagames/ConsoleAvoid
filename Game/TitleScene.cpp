#include "TitleScene.h"
#include "GameMain.h"

// <�V�[��>

TitleScene::TitleScene()
	: Scene()
{
	//SetMouseDispFlag(true);

	//auto& font = GameGlobal::GetInstance().font;

	//auto obj = GameObject::Create();
	//obj->transform()->position = Screen::GetBounds().Expand(-100).GetCenter();
	//obj->transform()->scale = Screen::GetBounds().Expand(-100).GetSize();
	//auto renderer = std::make_shared<BoxRenderer>();
	//renderer->material = Material{}.SetBase(Colors::Blue, .2f).SetBorder(Colors::White, 1.f, .5f);
	//obj->AddComponent<BoxRenderer>(renderer);

	//auto title = GameObject::Create("Title");
	//title->transform()->position = Screen::GetBounds().GetCenter() + Vector2{ -100, -50 };
	//title->AddNewComponent<FontTextRenderer>(GameGlobal::GetInstance().titlefont, "�^�C��");

	//auto text5 = GameObject::Create();
	//text5->transform()->position = Screen::GetBounds().GetCenter() + Vector2{ -200, 50 };
	//text5->AddNewComponent<FontTextRenderer>(font, "�N���b�N���X�y�[�X�L�[�ŃX�^�[�g�I");

	//auto director = GameObject::Create("GameDirector");
	//class GameDirector : public Component
	//{
	//	void Update()
	//	{
	//		if (InputManager::GetInstance().mouse->GetButtonDown(MOUSE_INPUT_1) || InputManager::GetInstance().key->GetButtonDown(KEY_INPUT_SPACE))
	//			SceneManager::GetInstance().RequestScene(SceneID::PLAY);
	//	}
	//};
	//director->AddNewComponent<GameDirector>();

	class TestObj : public Component
	{
		CXFont font_pong = CreateFontToHandle(CXFontType::CXFONT_PONG, 12);
		CXFont font = CreateFontToHandle(CXFontType::CXFONT_DEFAULT, 1);
		Vector2 obj_pos;

		void Start()
		{
			obj_pos = { 10, 10 };
		}

		void Update()
		{
			float speed = 100 * Time::deltaTime;
			if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
				obj_pos += Vector2::up * speed;
			if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
				obj_pos += Vector2::down * speed;
			if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
				obj_pos += Vector2::left * speed;
			if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
				obj_pos += Vector2::right * speed;
		}

		void Render()
		{
			static auto& context = ScreenManager::GetInstance().GetContext();

			Matrix3 world = Matrix3::CreateScale(Vector2{ 1, 2 });

			Vector2 boundsMax = GetWindowSize();
			DrawStringToHandle(boundsMax - obj_pos, L"Pong Game!", Colors::Red, &font_pong, world);
			for (int iy = 0; iy < 2; iy++)
				for (int ix = 0; ix < 10; ix++)
					DrawCircle(obj_pos + Vector2{ ix, iy }*10, 5, Colors::Blue, true, world);

			DrawStringToHandle(obj_pos, L"��su\nshi����\nsus\nhi������������", Colors::White, &font, world);
			DrawDashedLine(Vector2::zero, boundsMax, Colors::Green, 8, world);

			if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
				DrawStringToHandle(Vector2{ 15, 35 }, L"��", Colors::White, &font, world);
			if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
				DrawStringToHandle(Vector2{ 15, 45 }, L"��", Colors::White, &font, world);
			if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
				DrawStringToHandle(Vector2{ 10, 40 }, L"��", Colors::White, &font, world);
			if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
				DrawStringToHandle(Vector2{ 20, 40 }, L"��", Colors::White, &font, world);
		}
	};

	GameObject::Create("TestObj")->AddNewComponent<TestObj>();
}

TitleScene::~TitleScene()
{

}