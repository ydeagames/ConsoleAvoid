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

MouseInput::MouseInput()
{
}

MouseInput::~MouseInput()
{
}

void MouseInput::Consume(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	contextIn.mouse_handler.click_state_last ^= (contextIn.mouse_handler.click_state_last ^ contextIn.mouse_handler.click_state) & button;
}

bool MouseInput::GetButton(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	return (contextIn.mouse_handler.click_state & button) != 0;
}

bool MouseInput::GetButtonDown(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	return !(contextIn.mouse_handler.click_state_last & button) && (contextIn.mouse_handler.click_state & button);
}

bool MouseInput::GetButtonUp(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	return (contextIn.mouse_handler.click_state_last & button) && !(contextIn.mouse_handler.click_state & button);
}

bool MouseInput::GetButtonDoubleClick(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	return !(contextIn.mouse_handler.doubleclick_state_last & button) && (contextIn.mouse_handler.doubleclick_state & button);
}

void MouseInput::Update()
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	position = Vector2{ contextIn.mouse_handler.position } * ConsoleToScreen;
	Vector2 scale = Vector2::one * ConsoleToScreen - Vector2::zero * ConsoleToScreen;
	wheel_pos = contextIn.mouse_handler.wheel * scale.y;
	wheel_delta = contextIn.mouse_handler.delta_wheel * scale.y;
}

const Vector2& MouseInput::GetPosition()
{
	return position;
}

float MouseInput::GetWheel()
{
	return wheel_pos;
}

float MouseInput::GetDeltaWheel()
{
	return wheel_delta;
}

KeyInput::KeyInput()
{
}

KeyInput::~KeyInput()
{
}

void KeyInput::Update()
{
}

void KeyInput::Consume(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	contextIn.key_handler.key_state_last[button] = contextIn.key_handler.key_state[button];
}

bool KeyInput::GetButton(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	return contextIn.key_handler.key_state[button] != 0;
}

bool KeyInput::GetButtonDown(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	return contextIn.key_handler.key_state_last[button] == 0 && contextIn.key_handler.key_state[button] != 0;
}

bool KeyInput::GetButtonUp(int button)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();
	return contextIn.key_handler.key_state_last[button] != 0 && !contextIn.key_handler.key_state[button] == 0;
}

InputManager::InputManager()
{
	key = Register<KeyInput>("Key", std::make_shared<KeyInput>());
	mouse = Register<MouseInput>("Mouse", std::make_shared<MouseInput>());
}

void InputManager::Update()
{
	for (auto itr = inputsname.begin(); itr != inputsname.end(); ++itr)
		itr->second->Update();
}
