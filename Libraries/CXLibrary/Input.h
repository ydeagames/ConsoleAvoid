#pragma once

class Input;

class InputButton final
{
private:
	std::shared_ptr<Input> input;
	int button;

public:
	InputButton(std::shared_ptr<Input> input, int button);

	// ����
	void Consume() const;

	// ������Ă��邩
	bool GetButton() const;

	// �����ꂽ��
	bool GetButtonDown() const;

	// �����ꂽ��
	bool GetButtonUp() const;
};

// <���̓f�o�C�X>
class Input : public std::enable_shared_from_this<Input>
{
public:
	Input();

	virtual ~Input();

	// �X�V
	virtual void Update() = 0;

	// ����
	virtual void Consume(int button) = 0;

	// �{�^���n���h��
	InputButton GetInputButton(int button);

	// ������Ă��邩
	virtual bool GetButton(int button) = 0;

	// �����ꂽ��
	virtual bool GetButtonDown(int button) = 0;

	// �����ꂽ��
	virtual bool GetButtonUp(int button) = 0;
};

// <�}�E�X���̓f�o�C�X>
class MouseInput final : public Input
{
private:
	Vector2 position;
	float wheel_pos;
	float wheel_delta;

public:
	enum MouseInputButton
	{
		MOUSE_INPUT_0 = FROM_LEFT_1ST_BUTTON_PRESSED,
		MOUSE_INPUT_1 = RIGHTMOST_BUTTON_PRESSED,
		MOUSE_INPUT_2 = FROM_LEFT_2ND_BUTTON_PRESSED,
		MOUSE_INPUT_3 = FROM_LEFT_3RD_BUTTON_PRESSED,
		MOUSE_INPUT_4 = FROM_LEFT_4TH_BUTTON_PRESSED,
		MOUSE_INPUT_LEFT = MOUSE_INPUT_0,
		MOUSE_INPUT_RIGHT = MOUSE_INPUT_1,
		MOUSE_INPUT_MIDDLE = MOUSE_INPUT_2,
	};

public:
	MouseInput();

	~MouseInput();

	void Update() override;

	void Consume(int button) override;

	bool GetButton(int button) override;

	bool GetButtonDown(int button) override;

	bool GetButtonUp(int button) override;

	// �_�u���N���b�N���擾
	bool GetButtonDoubleClick(int button);

	// �}�E�X�����W���擾
	const Vector2& GetPosition();

	// �}�E�X�̉�]�ʂ��擾
	float GetWheel();

	// �}�E�X�̉�]�ʂ̍����擾
	float GetDeltaWheel();
};

// <�L�[���̓f�o�C�X>
class KeyInput final : public Input
{
public:
	KeyInput();

	virtual ~KeyInput();

	void Update() override;

	void Consume(int button) override;

	bool GetButton(int button) override;

	bool GetButtonDown(int button) override;

	bool GetButtonUp(int button) override;
};

// <���̓f�o�C�X�}�l�[�W��>
class InputManager final : public Singleton<InputManager>
{
private:
	std::unordered_map<std::string, std::shared_ptr<Input>> inputsname;
	std::unordered_map<std::type_index, std::string> inputstype;

public:
	std::shared_ptr<KeyInput> key;
	std::shared_ptr<MouseInput> mouse;

private:
	InputManager();
	friend class Singleton<InputManager>;

public:
	template<class T> std::shared_ptr<T> Register(const std::string& name, const std::shared_ptr<T> input)
	{
		inputsname[name] = input;
		if (inputstype.count(typeid(T)) <= 0)
			inputstype[typeid(T)] = name;
		return input;
	}

	inline void Unregister(const std::string& name)
	{
		inputsname.erase(name);
	}

	template<class T> std::shared_ptr<T> GetInput(const std::string& name)
	{
		return std::dynamic_pointer_cast<T, Input>(inputsname[name]);
	}

	template<class T> std::shared_ptr<T> GetInput()
	{
		return GetInput<T>(inputstype[typeid(T)]);
	}

	void Update();
};
