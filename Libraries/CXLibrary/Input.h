#pragma once

class Input;

class InputButton final
{
private:
	std::shared_ptr<Input> input;
	int button;

public:
	InputButton(std::shared_ptr<Input> input, int button);

	// 消費
	void Consume() const;

	// 押されているか
	bool GetButton() const;

	// 押されたか
	bool GetButtonDown() const;

	// 離されたか
	bool GetButtonUp() const;
};

// <入力デバイス>
class Input : public std::enable_shared_from_this<Input>
{
public:
	Input();

	virtual ~Input();

	// 更新
	virtual void Update() = 0;

	// 消費
	virtual void Consume(int button) = 0;

	// ボタンハンドラ
	InputButton GetInputButton(int button);

	// 押されているか
	virtual bool GetButton(int button) = 0;

	// 押されたか
	virtual bool GetButtonDown(int button) = 0;

	// 離されたか
	virtual bool GetButtonUp(int button) = 0;
};

// <マウス入力デバイス>
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

	// ダブルクリックを取得
	bool GetButtonDoubleClick(int button);

	// マウスを座標を取得
	const Vector2& GetPosition();

	// マウスの回転量を取得
	float GetWheel();

	// マウスの回転量の差を取得
	float GetDeltaWheel();
};

// <キー入力デバイス>
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

// <入力デバイスマネージャ>
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
