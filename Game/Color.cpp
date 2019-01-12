#include "Color.h"

Attributes::Attributes(Color background, Color foreground)
	: background(background)
	, foreground(foreground)
{
}

Attributes::Attributes(const WORD& word)
	: foreground(static_cast<Color>((word >> 0) & 0xf))
	, background(static_cast<Color>((word >> 4) & 0xf))
{
}

Attributes::operator WORD() const
{
	return (background << 4) | foreground;
}
