#pragma once
#include "Color.h"
#include "ScreenInputContext.h"
#include "ScreenOutputContext.h"

class ScreenManager : public Singleton<ScreenManager>
{
public:
	static const SHORT PixelAspectRatio = 2;

private:
	ScreenInputContext inputContext;
	ScreenOutputContext outputContext;

private:
	ScreenManager();
	friend class Singleton<ScreenManager>;

public:
	inline ScreenInputContext& GetInputContext() { return inputContext; }
	inline ScreenOutputContext& GetOutputContext() { return outputContext; }

public:
	void UpdateOutputContext();
	bool SetScreenSize(int width, int height);
	bool SetWindowSize(int width, int height);
	void SetFontSize(int size);
	void SetPixelSize(int size);
	void SetDefaultText(WCHAR ch = L' ');
	void SetDefaultBackground(Color background);
	void SetDefaultForeground(Color foreground);
	void SetBackground(Color color);
	void SetForeground(Color color);
	void SetCursorVisibility(bool isVisible); // カーソルの表示・非表示切り替え
	void SetTitle(LPCWSTR title);
	void SetCursorPosition(COORD pos);

public:
	void EnableMouseInput();
};
