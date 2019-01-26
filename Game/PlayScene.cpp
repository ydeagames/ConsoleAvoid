#include "PlayScene.h"

float PlayScene::score = 0;

PlayScene::PlayScene()
{
	// �Q�[���̑z��T�C�Y
	static constexpr auto gameAspect = Vector2{ 16, 9 };
	// �Q�[���̑z��c����
	static constexpr float gameAspectRatio = gameAspect.x / gameAspect.y;
	// �t�B�[���h�T�C�Y
	static auto fieldSize = Vector2{ gameAspectRatio, 1.f };
	// �t�B�[���h�T�C�Y�̃}�[�W��
	static auto fieldMargin = Vector2::one * .1f;

	// �X�R�A��������
	score = 0;

	// �v���C���[�R���|�[�l���g
	class Player : public Component
	{
		const float speed = .4f;	// �L�[����ł̃X�s�[�h
		Vector2 last_pos;			// �Ō�̈ʒu
		Vector2 last_click;			// �Ō�̃N���b�N���W
		bool dragged;				// �h���b�O����Ă��邩

		bool lastBombed;			// �O�t���[���͔����Ă�����
		bool bombed;				// ��������
		Timer bombedTimer;			// �����Ă���̎���

		void Start()
		{
			dragged = false;
			bombed = lastBombed = false;

			bool& bombedref = bombed;
			// �C�x���g�o�X�� �΂̋ʂɐG�ꂽ�Ƃ��̃C�x���g ��o�^
			gameObject()->eventbus()->Register([&bombedref](TriggerEnterEvent& eventobj) {
				// �C�x���g�����΂����Ƃ��̏���
				//_RPT0(_CRT_WARN, "hit");
				// ���������t���O��ON
				bombedref = true;
			});
		}

		void Update()
		{
			// ����������
			if (bombed)
			{
				// ��������Ȃ��
				if (!lastBombed)
				{
					// ������̃^�C�}�[���X�^�[�g
					bombedTimer = Timer{}.Start(2.f);
					// �����A�j���[�V����
					gameObject()->GetComponent<TextureRenderer>()->texture = Texture{ std::vector<CXImage>{
							LoadGraph("Resources/Textures/bomb_explosion1.ppm", Transparent::FirstColor),
							LoadGraph("Resources/Textures/bomb_explosion2.ppm", Transparent::FirstColor),
							LoadGraph("Resources/Textures/bomb_explosion3.ppm", Transparent::FirstColor),
					},
					2.f / 3,
					false
					};
					// Rigidbody�̑��x��0�ɂ���
					gameObject()->GetComponent<Rigidbody>()->vel = Vector2::zero;
					lastBombed = true;
				}
				// �����Ă��玞�Ԃ���������
				if (bombedTimer.IsFinished())
					// ���U���g�V�[����
					SceneManager::GetInstance().RequestScene(SceneID::RESULT);
			}
			// �ꎞ��~���Ă��Ȃ�������
			else if (Time::speed > 0)
			{
				auto& rigidbody = gameObject()->GetComponent<Rigidbody>();

				// �L�[��������Rigidbody�̑��x��ǉ�
				rigidbody->vel = Vector2::zero;
				if (InputManager::GetInstance().key->GetButton('W') || InputManager::GetInstance().key->GetButton(VK_UP))
					rigidbody->vel += Vector2::up * speed;
				if (InputManager::GetInstance().key->GetButton('S') || InputManager::GetInstance().key->GetButton(VK_DOWN))
					rigidbody->vel += Vector2::down * speed;
				if (InputManager::GetInstance().key->GetButton('A') || InputManager::GetInstance().key->GetButton(VK_LEFT))
					rigidbody->vel += Vector2::left * speed;
				if (InputManager::GetInstance().key->GetButton('D') || InputManager::GetInstance().key->GetButton(VK_RIGHT))
					rigidbody->vel += Vector2::right * speed;

				// �J�[�\���̃��[���h���W���擾
				auto& transform = gameObject()->transform();
				auto& parentInverse = transform->GetParentMatrix().Inverse();
				auto point = InputManager::GetInstance().mouse->GetPosition() * parentInverse;
				// ���N���b�N���ꂽ��h���b�O�\
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

				// �͈͊O�Ɉړ����Ȃ��悤�ɂ���
				auto& collider = gameObject()->GetComponent<BoxCollider>();
				Bounds bounds = Bounds::CreateFromCenter(Vector2::zero, fieldSize).Expand(-transform->scale * collider->shape.size / 2);
				transform->position = bounds.ClosestPoint(transform->position);
			}
		}
	};

	// �t�B�[���h�R���|�[�l���g(�q�I�u�W�F�N�g�͂��ׂăt�B�[���h���W�ɕϊ������)
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
	// �t�B�[���h�I�u�W�F�N�g(�q�I�u�W�F�N�g�͂��ׂăt�B�[���h���W�ɕϊ������)
	auto field = GameObject::Create("Field");
	field->AddNewComponent<Canvas>();
	field->transform()->static_object = true;	// �ÓI�I�u�W�F�N�g

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
	back->AddNewComponent<TextureRenderer>(Texture{ LoadGraph("Resources/Textures/back.ppm") });	// ���U���g��ʂœh��Ԃ�
	back->transform()->static_object = true;														// �ÓI�`��

	//// �t�B�[���h�̋��E��
	//auto border = GameObject::Create("Border");
	//border->transform()->parent = field->transform();
	//border->transform()->scale = Vector2{ gameAspectRatio, 1.f };
	//border->transform()->static_object = true;
	//border->AddNewComponent<BoxRenderer>()->material = Material{}.SetBorder(Colors::Red);

	// �΂̋ʃR���g���[���[�R���|�[�l���g
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

	// �΂̋ʃW�F�l���[�^�[�R���|�[�l���g
	class FireGenerator : public Component
	{
	public:
		// �Q�[�����n�܂��Ă��邩
		bool started;

	private:
		float SpeedMultiply = 1.001f;	// 1�e���˂��Ƃ̃X�s�[�h������
		float speed = .1f;				// �e�̃X�s�[�h
		float IntervalMultiply = .97f;	// 1�e���˂��Ƃ̔��ˊԊu������
		float interval = 1.5f;			// �e�̔��ˊԊu
		float minInterval = .25f;		// �e�̍ŏ����ˊԊu
		Timer timer;					// �e�̃^�C�}�[
		Texture texture;				// �e�̃e�N�X�`��

		void Start()
		{
			started = false;
			timer = Timer{}.Start(interval);
			texture = Texture{ LoadGraph("Resources/Textures/fire.ppm", Transparent::FirstColor) };		// �΂̋ʂ̃e�N�X�`��
		}

		void Update()
		{
			// �Q�[�����n�܂��Ă��Ĉ��Ԋu���Ƃ�
			if (started && timer.IsFinished())
			{
				// �����͓����ŉ�ʒ[����
				float direction = Random::Range(0.f, MathUtils::ToRadians(360));
				auto size = fieldSize / 2 + fieldMargin;
				auto vec = (Vector2::right * size.Length()).Rotate(direction);
				auto trace = (size.y / size.x < std::abs(vec.y / vec.x)) ? vec * std::abs(size.y / vec.y) : vec * std::abs(size.x / vec.x);
				float vecdirection = Random::Range(-MathUtils::ToRadians(90), MathUtils::ToRadians(90));
				auto vecnorm = (size.y / size.x < std::abs(vec.y / vec.x)) ? (vec.y < 0 ? Vector2::up : Vector2::down) : (vec.x < 0 ? Vector2::left : Vector2::right);
				auto vel = (-vecnorm * speed).Rotate(vecdirection);

				// �΂̋ʃI�u�W�F�N�g
				auto fire = GameObject::Create("Fire", 2);
				fire->AddNewComponent<TextureRenderer>(texture);													// �e�N�X�`��
				fire->transform()->parent = GameObject::Find("Field")->transform();									// �e�I�u�W�F�N�g
				fire->transform()->position = trace;																// ��ʒ[
				fire->transform()->scale = Vector2::one * .1f;														// �T�C�Y
				fire->AddNewComponentAs<Collider, CircleCollider>(Circle{ Vector2::zero, .5f })->isTrigger = true;	// �R���C�_�[
				fire->AddNewComponent<Rigidbody>()->vel = vel;														// Rigidbody
				fire->AddNewComponent<FireController>();															// �R���g���[���[

				interval = std::max(minInterval, interval * IntervalMultiply);										// �C���^�[�o����Z��
				speed *= SpeedMultiply;																				// �X�s�[�h������
				timer.Start(interval);																				// �^�C�}�[���Z�b�g
			}
		}
	};
	// �΂̋ʃW�F�l���[�^�[�I�u�W�F�N�g
	auto firegen = GameObject::Create("FireGenerator");
	firegen->transform()->parent = field->transform();
	firegen->AddNewComponent<FireGenerator>();

	// �v���C���[�I�u�W�F�N�g
	auto player = GameObject::Create("Player", 3);
	player->transform()->parent = field->transform();													  // �e�I�u�W�F�N�g
	player->transform()->scale = Vector2::one * .3f;													  // �T�C�Y
	player->transform()->position = Vector2{ 0.f, .3f };												  // �ʒu�͒��S���班����
	player->AddNewComponent<Player>();																	  // �v���C���[
	player->AddNewComponent<Rigidbody>(Vector2{}, std::vector<int>{ 2 });								  // ���C���[2(�΂̋�)�ƏՓ˂���Rigidbody
	player->AddNewComponentAs<Collider, BoxCollider>(Box{ Vector2{ 0.f, .1f }, Vector2{ .4f, .5f } });	  //�@�R���C�_�[
	auto texture = Texture{
		std::vector<CXImage>{
			LoadGraph("Resources/Textures/bomb1.ppm"),
			LoadGraph("Resources/Textures/bomb2.ppm", Transparent::FirstColor),
		},
		1
	};		// �e�N�X�`��
	player->AddNewComponent<TextureRenderer>(texture);
	//player->AddNewComponent<BoxRenderer>()->material = Material{}.SetBorder(Colors::Blue);

	// �X�R�A�p�l���R���|�[�l���g
	class ScorePanel : public Component
	{
		void Update()
		{
			// �X�R�A�X�V
			std::wstring str = String::Format(L"SCORE: %.0f", score);		// �����w��q�ŃX�R�A�����񐶐�
			gameObject()->GetComponent<FontTextRenderer>()->text = str;
		}
	};
	// �X�R�A�p�l���I�u�W�F�N�g
	auto scorepanel = GameObject::Create("ScorePanel", 5);
	scorepanel->AddNewComponent<ScorePanel>();
	scorepanel->transform()->parent = field->transform();														  // �e�I�u�W�F�N�g
	scorepanel->transform()->position = Vector2{ -gameAspectRatio / 2 + .1f, -.5f + .2f };						  // ����
	scorepanel->transform()->scale = Vector2{ .0125f, .0125f };													  // �T�C�Y
	scorepanel->AddNewComponent<FontTextRenderer>(CreateFontToHandle(CXFontType::CXFONT_PONG, 4), L"SCORE");	  // �t�H���g

	// �J�E���g�_�E���R���|�[�l���g
	class Countdown : public Component
	{
		Timer timer;					  // 3�b�J�E���^�[

		void Start()
		{
			// �^�C�}�[�X�^�[�g
			timer = Timer{}.Start(3);
		}

		void Update()
		{
			// �I�����Ă�����
			if (timer.IsFinished())
			{
				// �t�@�C�A�W�F�l���[�^�[���J�n
				GameObject::Find("FireGenerator")->GetComponent<FireGenerator>()->started = true;
				// �I�u�W�F�N�g������
				gameObject()->Destroy();
			}
		}
	};
	// �J�E���g�_�E���I�u�W�F�N�g
	auto countdown = GameObject::Create("Countdown", 5);
	countdown->AddNewComponent<Countdown>();
	countdown->transform()->parent = field->transform();
	countdown->transform()->position = Vector2::zero;
	countdown->transform()->scale = Vector2{ .2f, .2f };
	// �J�E���g�_�E���A�j���[�V����
	countdown->AddNewComponent<TextureRenderer>(Texture{ std::vector<CXImage>{
			LoadGraph("Resources/Textures/countdown3.ppm", Transparent::FirstColor),
			LoadGraph("Resources/Textures/countdown2.ppm", Transparent::FirstColor),
			LoadGraph("Resources/Textures/countdown1.ppm", Transparent::FirstColor),
	},
	1,
	false
	});

	// �|�[�Y�R���|�[�l���g
	class Pause : public Component
	{
		// �|�[�Y����Ă���
		bool paused;

		void Start()
		{
			paused = false;
		}

		void Update()
		{
			// �n�܂��Ă��āA�X�y�[�X�L�[�Ń|�[�Y
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
				// �w�i�𔼓���
				auto& transform = gameObject()->transform();
				auto windowsize = GetWindowSize();
				DrawBox(Vector2::zero, windowsize, Colors::Black, true, Matrix3::Identity, .5f);
			}
		}
	};
	// �|�[�Y�I�u�W�F�N�g
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
