#include "LogoScene.h"

// ���S�V�[��
LogoScene::LogoScene()
{
	// �w�i�摜�R���|�[�l���g
	class Back : public Component
	{
		void Update()
		{
			// ��ʃT�C�Y�����ς��ɂ���
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = windowsize;
			transform->position = windowsize / 2;

			//static auto& context = ScreenManager::GetInstance().GetOutputContext();
			//static auto gameAspect = Vector2{ 240, 151 };
			//auto size = AspectUtils::Outer(gameAspect, windowsize);
			//int fontsize = static_cast<int>(std::round(size.y / gameAspect.y * 7 / 3)) * 3;
			//if (context.fontSize != fontsize)
			//	ScreenManager::GetInstance().SetPixelSize(fontsize);
		}
	};
	// �w�i�摜�I�u�W�F�N�g
	auto& back = GameObject::Create("Back");
	back->AddNewComponent<Back>();														// �w�i�摜�R���|�[�l���g
	back->AddNewComponent<BoxRenderer>()->material = Material{}.SetBase(Colors::White);	// �����h��Ԃ�
	back->transform()->static_object = true;											// �ÓI�`��

	// ���S�R���|�[�l���g
	class Logo : public Component
	{
		// ����
		float time = 0;

		void Update()
		{
			// ����
			time += .01f;
			if (time > 1.f)
				// ���Ԍo�߂Ń^�C�g���V�[����
				SceneManager::GetInstance().RequestScene(SceneID::TITLE);

			// �c������ێ����Ȃ��珙�X�ɑ傫�����Ă���
			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();		// ���S�e�N�X�`���̑傫���擾
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));			// 1:1�̃A�X�y�N�g��ŉ摜�̘g�Ɏ��܂�T�C�Y
			transform->scale = size * Easings::OUTCUBIC.GetEasingValueRange(std::min(1.f, time), .5f, .75f);	// OutCubic�C�[�W���O���g�p���ĉ摜���g��
			transform->position = windowsize / 2;
		}
	};
	// ���S�I�u�W�F�N�g
	auto& logo = GameObject::Create("Logo");
	logo->AddNewComponent<Logo>();																				// ���S�R���|�[�l���g
	logo->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/ydeagames.ppm") });			// ���S�e�N�X�`���`��

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

LogoScene::~LogoScene()
{
}
