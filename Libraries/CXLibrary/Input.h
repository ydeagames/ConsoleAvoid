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

// <キー入力デバイス>
class KeyInput final : public Input
{
private:
	static const DWORD MAX_INPUT_LENGTH = 16;

	bool input_state[256];
	bool input_state_last[256];

	INPUT_RECORD input[MAX_INPUT_LENGTH];
	HANDLE console;
	DWORD length;

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
