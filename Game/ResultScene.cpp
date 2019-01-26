#include "ResultScene.h"
#include "PlayScene.h"

// ���U���g�V�[��
ResultScene::ResultScene()
{
	// �Q�[���̑z��T�C�Y
	static constexpr auto gameAspect = Vector2{ 16, 9 };
	// �Q�[���̑z��c����
	static constexpr float gameAspectRatio = gameAspect.x / gameAspect.y;

	// �w�i�摜�R���|�[�l���g
	class Back : public Component
	{
		void Update()
		{
			// ��ʃT�C�Y�����ς��ɂ���
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = AspectUtils::Outer(Vector2::one, windowsize);
			transform->position = windowsize / 2;
		}
	};
	// �w�i�摜�I�u�W�F�N�g
	auto& back = GameObject::Create("Back");
	back->AddNewComponent<Back>();																	// �w�i�摜�R���|�[�l���g
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/titlebg.ppm") });	// ���U���g��ʂœh��Ԃ�
	back->transform()->static_object = true;														// �ÓI�`��

	// ���U���gUI�R���|�[�l���g																		
	class Result : public Component
	{
		void Update()
		{
			// �c������ێ����Ȃ�����܂�͈͍ő�\��
			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();	// ���U���gUI�e�N�X�`���̑傫���擾
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));		// 1:1�̃A�X�y�N�g��ŉ摜�̘g�Ɏ��܂�T�C�Y
			transform->scale = size;																		// �ő剻
			transform->position = windowsize / 2;
		}
	};
	// ���U���gUI�I�u�W�F�N�g
	auto& result = GameObject::Create("Result");
	result->AddNewComponent<Result>();																							// ���U���gUI�R���|�[�l���g
	result->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/result.ppm", Transparent::FirstColor) });	// ���U���gUI�e�N�X�`���`��
	result->transform()->static_object = true;

	// �X�R�A�I�u�W�F�N�g
	auto scorepanel = GameObject::Create("ScorePanel", 5);
	scorepanel->transform()->parent = back->transform();			// �e�I�u�W�F�N�g
	scorepanel->transform()->position = Vector2{ -.2f, .1f };		// ���[�J�����W
	scorepanel->transform()->scale = Vector2{ .005f, .005f };		// ���[�J���X�P�[��
	scorepanel->AddNewComponent<FontTextRenderer>(CreateFontToHandle(CXFontType::CXFONT_PONG, 4), String::Format(L"SCORE: %.0f", PlayScene::score));

	// �N���b�N�A�C�R���R���|�[�l���g
	class Click : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = Vector2::one * 20;
			transform->position = Vector2{ 10.f, windowsize.y - 10.f };	// ����
		}
	};
	// �N���b�N�A�C�R���I�u�W�F�N�g
	auto& click = GameObject::Create("Click");
	click->AddNewComponent<Click>();
	click->AddNewComponent<TextureRenderer>(Texture{
		std::vector<CXImage>{
			LoadGraph("Resources/Textures/click1.ppm", Transparent::FirstColor),
			LoadGraph("Resources/Textures/click2.ppm", Transparent::FirstColor)
		}, .5f
		});		// �N���b�N�A�j���[�V����

	// �V�[���t�b�N�R���|�[�l���g
	class SceneHook : public Component
	{
		void Update()
		{
			// �X�y�[�X�L�[���}�E�X���{�^���������ꂽ��
			if (InputManager::GetInstance().key->GetButtonDown(VK_SPACE)
				|| InputManager::GetInstance().mouse->GetButtonDown(MouseInput::MOUSE_INPUT_LEFT))
				// �^�C�g���V�[���ֈړ�
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);
		}
	};
	// �V�[���t�b�N�I�u�W�F�N�g
	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

ResultScene::~ResultScene()
{
}
