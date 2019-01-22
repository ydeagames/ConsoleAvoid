#pragma once

class ScreenInputContext
{
public:
	static constexpr DWORD MAX_INPUT_LENGTH = 16;	// 最大イベント数

public:
	HANDLE handle;									// 入力デバイスのハンドル
	INPUT_RECORD input[MAX_INPUT_LENGTH];			// 未処理のイベント
	DWORD length;									// 未処理のイベント数

public:
	class KeyEventHandler
	{
	public:
		bool key_state[256];
		bool key_state_last[256];

	public:
		KeyEventHandler()
			: key_state()
			, key_state_last() {}

	public:
		void UpdateEvent();
		void HandleEvent(const KEY_EVENT_RECORD& event);
	} key_handler;

	class MouseEventHandler
	{
	public:
		COORD position;
		DWORD click_state;
		DWORD click_state_last;
		DWORD doubleclick_state;
		DWORD doubleclick_state_last;
		int delta_wheel;
		int wheel;
		int delta_hwheel;
		int hwheel;

	public:
		MouseEventHandler()
			: click_state(0)
			, click_state_last(0)
			, doubleclick_state(0)
			, doubleclick_state_last(0) {}

	public:
		void UpdateEvent();
		void HandleEvent(const MOUSE_EVENT_RECORD& event);
	} mouse_handler;

	class WindowBufferSizeEventHandler
	{
	public:
		void UpdateEvent();
		void HandleEvent(const WINDOW_BUFFER_SIZE_RECORD& event);
	} window_buffer_size_handler;

	class MenuEventHandler
	{
	public:
		void UpdateEvent();
		void HandleEvent(const MENU_EVENT_RECORD& event);
	} menu_handler;

	class FocusEventHandler
	{
	public:
		void UpdateEvent();
		void HandleEvent(const FOCUS_EVENT_RECORD& event);
	} focus_handler;

public:
	ScreenInputContext(const HANDLE& handle);

public:
	void HandleInput();
};