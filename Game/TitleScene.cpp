#include "TitleScene.h"

// �^�C�g���V�[��
TitleScene::TitleScene()
{
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
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/titlebg.ppm") });	// �^�C�g���摜�œh��Ԃ�
	back->transform()->static_object = true;														// �ÓI�`��

	// �^�C�g��UI�R���|�[�l���g
	class Title : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto& imagesize = gameObject()->GetComponent<TextureRenderer>()->texture.GetFrame().GetSize();	// ���U���gUI�e�N�X�`���̑傫���擾
			auto windowsize = GetWindowSize();
			auto size = AspectUtils::Outer(Vector2::one, AspectUtils::Inner(imagesize, windowsize));		// 1:1�̃A�X�y�N�g��ŉ摜�̘g�Ɏ��܂�T�C�Y
			transform->scale = size;																		// �ő剻
			transform->position = windowsize / 2;
		}
	};
	// �^�C�g��UI�I�u�W�F�N�g
	auto& title = GameObject::Create("Title");
	title->AddNewComponent<Title>();																						  // ���U���gUI�R���|�[�l���g
	title->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/title.ppm", Transparent::FirstColor) });	  // ���U���gUI�e�N�X�`���`��
	title->transform()->static_object = true;

	// �N���b�N�A�C�R���R���|�[�l���g
	class Click : public Component
	{
		void Update()
		{
			auto& transform = gameObject()->transform();
			auto windowsize = GetWindowSize();
			transform->scale = Vector2::one * 20;
			transform->position = Vector2{ 10.f, windowsize.y - 10.f };	 // ����
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
				SceneManager::GetInstance().RequestScene(SceneID::PLAY);
		}
	};
	// �V�[���t�b�N�I�u�W�F�N�g
	GameObject::Create("SceneHook")->AddNewComponent<SceneHook>();
}

TitleScene::~TitleScene()
{
}
