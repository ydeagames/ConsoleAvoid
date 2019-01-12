#pragma once

// 色
using Color = int;

// 色セット
namespace Colors
{
	enum : Color
	{
		Default = -1,
		Black,
		DarkBlue,
		DarkGreen,
		DarkCyan,
		DarkRed,
		DarkMagenta,
		DarkYellow,
		Gray,
		DarkGray,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Yellow,
		White,
	};
}

// 属性(色)
class Attributes
{
private:
	Color background;	// 背景カラー
	Color foreground;	// テキストカラー

public:
	Attributes(Color background, Color foreground)
		: background(background)
		, foreground(foreground) {};
	Attributes()
		: Attributes(Colors::Default, Colors::Default) {};
	Attributes(const WORD& word)
		: Attributes(static_cast<Color>((word >> 4) & 0xf), static_cast<Color>((word >> 0) & 0xf)) {}

public:
	constexpr inline operator WORD() const { return (background << 4) | foreground; }

	inline Attributes& back(Color color = Colors::Default) { background = color & 0xf; return *this; }
	inline Attributes& text(Color color = Colors::Default) { foreground = color & 0xf; return *this; }

	inline Attributes operator|(const Attributes& attr)
	{
		return{ attr.background<0 ? background : attr.background, attr.foreground<0 ? foreground : attr.foreground };
	}
};
