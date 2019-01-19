#include "ScreenContext.h"

ScreenOutputContext::ScreenOutputContext(const HANDLE& hOut, CHAR_INFO default_pixel)
	: handle(hOut)
	, boundsMin(COORD{ 0, 0 })
	, default_pixel(default_pixel)
{
	// スクリーンバッファに関する情報
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	// スクリーンバッファに関する情報の取得
	GetConsoleScreenBufferInfo(handle, &screenBufferInfo);

	// コンソール画面に関する情報の設定
	handle = handle;
	boundsMax = screenBufferInfo.dwSize;
	screenRegion = { boundsMin.X, boundsMin.Y, boundsMax.X - 1, boundsMax.Y - 1 };
	attributes = screenBufferInfo.wAttributes;

	buffer = std::vector<CHAR_INFO>(boundsMax.X * boundsMax.Y, default_pixel);
	bufferPtr = &buffer[0];
}

// 画面を初期化する
void ScreenOutputContext::Clear()
{
	// オフスクリーンをデフォルトピクセルで初期化
	int length = static_cast<int>(boundsMax.X * boundsMax.Y);
	std::fill_n(buffer.begin(), length, default_pixel);
}

void ScreenOutputContext::Flush()
{
	WriteConsoleOutputW(handle, bufferPtr, boundsMax, boundsMin, &screenRegion);
}

ScreenInputContext::ScreenInputContext(const HANDLE & handle)
	: handle(handle)
{
}

void ScreenInputContext::HandleInput()
{
	key_handler.UpdateEvent();
	mouse_handler.UpdateEvent();
	window_buffer_size_handler.UpdateEvent();
	menu_handler.UpdateEvent();
	focus_handler.UpdateEvent();

	GetNumberOfConsoleInputEvents(handle, &length);

	int iLength = static_cast<int>(length);
	if (iLength > 0)
	{
		ReadConsoleInputW(handle, input, MAX_INPUT_LENGTH, &length);

		for (int i = 0; i < iLength; i++)
		{
			auto& in = input[i];

			switch (in.EventType)
			{
			case KEY_EVENT:
				key_handler.HandleEvent(in.Event.KeyEvent);
				break;
			case MOUSE_EVENT:
				mouse_handler.HandleEvent(in.Event.MouseEvent);
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				window_buffer_size_handler.HandleEvent(in.Event.WindowBufferSizeEvent);
				break;
			case MENU_EVENT:
				menu_handler.HandleEvent(in.Event.MenuEvent);
				break;
			case FOCUS_EVENT:
				focus_handler.HandleEvent(in.Event.FocusEvent);
				break;
			}
		}
	}
}

void ScreenInputContext::KeyEventHandler::UpdateEvent()
{
	memcpy(key_state_last, key_state, sizeof(key_state));
}

void ScreenInputContext::KeyEventHandler::HandleEvent(const KEY_EVENT_RECORD & event)
{
	key_state[event.wVirtualKeyCode] = event.bKeyDown != 0;
}

void ScreenInputContext::MouseEventHandler::UpdateEvent()
{
	click_state_last = click_state;
	doubleclick_state_last = doubleclick_state;
	delta_wheel = 0;
	delta_hwheel = 0;
}

void ScreenInputContext::MouseEventHandler::HandleEvent(const MOUSE_EVENT_RECORD & event)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif

	switch (event.dwEventFlags)
	{
	case 0:
		click_state = event.dwButtonState;
		break;
	case MOUSE_MOVED:
		position = event.dwMousePosition;
		break;
	case DOUBLE_CLICK:
		doubleclick_state = event.dwButtonState;
		break;
	case MOUSE_WHEELED:
	{
		int delta = (static_cast<int>(event.dwButtonState) >> 16) > 0 ? 1 : -1;
		delta_wheel += delta;
		wheel += delta;
		break;
	}
	case MOUSE_HWHEELED:
	{
		int delta = (static_cast<int>(event.dwButtonState) >> 16) > 0 ? 1 : -1;
		delta_hwheel += delta;
		hwheel += delta;
		break;
	}
	}
}

void ScreenInputContext::WindowBufferSizeEventHandler::UpdateEvent()
{
}

void ScreenInputContext::WindowBufferSizeEventHandler::HandleEvent(const WINDOW_BUFFER_SIZE_RECORD & event)
{
	ScreenManager::GetInstance().UpdateOutputContext();
}

void ScreenInputContext::MenuEventHandler::UpdateEvent()
{
}

void ScreenInputContext::MenuEventHandler::HandleEvent(const MENU_EVENT_RECORD & event)
{
}

void ScreenInputContext::FocusEventHandler::UpdateEvent()
{
}

void ScreenInputContext::FocusEventHandler::HandleEvent(const FOCUS_EVENT_RECORD & event)
{
}
