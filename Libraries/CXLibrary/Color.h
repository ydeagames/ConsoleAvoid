#pragma once

// �F
using Color = int;
using ColorCode = unsigned int;

// �F�Z�b�g
namespace Colors
{
	enum : Color
	{
		Transparent = -1,
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

	constexpr ColorCode ColorCodes[16] =
	{
		0x000000,
		0x000080,
		0x008000,
		0x008080,
		0x800000,
		0x800000,
		0x800000,
		0xc0c0c0,
		0x808080,
		0x0000ff,
		0x00ff00,
		0x00ffff,
		0xff0000,
		0xff00ff,
		0xffff00,
		0xffffff,
	};
}

// ����(�F)
class Attributes
{
private:
	Color background;	// �w�i�J���[
	Color foreground;	// �e�L�X�g�J���[

public:
	Attributes(Color background, Color foreground)
		: background(background & 0xf)
		, foreground(foreground & 0xf) {};
	Attributes(const WORD& word)
		: Attributes(static_cast<Color>((word >> 4)), static_cast<Color>((word >> 0))) {}

public:
	constexpr inline operator WORD() const { return (background << 4) | foreground; }

	inline Attributes& back(Color color) { background = color & 0xf; return *this; }
	inline Attributes& text(Color color) { foreground = color & 0xf; return *this; }

	inline Attributes operator|(const Attributes& attr)
	{
		return{ attr.background<0 ? background : attr.background, attr.foreground<0 ? foreground : attr.foreground };
	}

public:
	static constexpr inline WORD color(Color background, Color foreground) { return ((background & 0xf) << 4) | (foreground & 0xf); }
	static constexpr inline WORD back(WORD attributes, Color background) { return (attributes & 0x0f) | ((background & 0xf) << 4); }
	static constexpr inline WORD text(WORD attributes, Color foreground) { return (attributes & 0xf0) | ((foreground & 0xf) << 0); }
};
