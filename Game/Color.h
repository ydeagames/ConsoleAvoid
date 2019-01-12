#pragma once

// 色
using Color = int;

// 色セット
namespace Colors
{
	enum : Color
	{
		Black,
		Dark_blue,
		Dark_green,
		Dark_cyan,
		Dark_red,
		Dark_magenta,
		Dark_yellow,
		Gray,
		Dark_gray,
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
public:
	Color background;	// 背景カラー
	Color foreground;	// テキストカラー

public:
	// 属性(色)を作成
	Attributes(Color background, Color foreground = Colors::White);
	Attributes(const WORD& word);
	operator WORD() const;
};
