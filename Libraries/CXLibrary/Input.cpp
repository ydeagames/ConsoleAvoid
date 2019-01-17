#include "Input.h"
#include "ScreenManager.h"

InputButton::InputButton(std::shared_ptr<Input> input, int button) :
	input(input),
	button(button)
{
}

void InputButton::Consume() const
{
	input->Consume(button);
}

bool InputButton::GetButton() const
{
	return input->GetButton(button);
}

bool InputButton::GetButtonDown() const
{
	return input->GetButtonDown(button);
}

bool InputButton::GetButtonUp() const
{
	return input->GetButtonUp(button);
}

Input::Input()
{
}

Input::~Input()
{
}

// ボタンハンドラ
InputButton Input::GetInputButton(int button)
{
	return{ shared_from_this(), button };
}

KeyInput::KeyInput()
{
	console = GetStdHandle(STD_INPUT_HANDLE);
	length = 0;
	memset(input_state, false, sizeof(input_state));
}

KeyInput::~KeyInput()
{
}

void KeyInput::Update()
{
	memcpy(input_state_last, input_state, sizeof(input_state));
	
	GetNumberOfConsoleInputEvents(console, &length);
	if (static_cast<int>(length) > 0)
	{
		ReadConsoleInput(console, input, MAX_INPUT_LENGTH, &length);
		for (int i = 0; i < static_cast<int>(length); i++)
		{
			auto& in = input[i];

			switch (in.EventType)
			{
			case KEY_EVENT:
				input_state[in.Event.KeyEvent.wVirtualKeyCode] = in.Event.KeyEvent.bKeyDown != 0;
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				ScreenManager::GetInstance().UpdateContext();
				break;
			}
		}
	}
}

void KeyInput::Consume(int button)
{
	input_state_last[button] = input_state[button];
}

bool KeyInput::GetButton(int button)
{
	return input_state[button] != 0;
}

bool KeyInput::GetButtonDown(int button)
{
	return input_state_last[button] == 0 && input_state[button] != 0;
}

bool KeyInput::GetButtonUp(int button)
{
	return input_state_last[button] != 0 && !input_state[button] == 0;
}

InputManager::InputManager()
{
	key = Register<KeyInput>("Key", std::make_shared<KeyInput>());
}

void InputManager::Update()
{
	for (auto itr = inputsname.begin(); itr != inputsname.end(); ++itr)
		itr->second->Update();
}
