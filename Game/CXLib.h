#pragma once
#include "Color.h"
#include "Vector2.h"
#include "Matrix.h"

namespace CXLib
{
	// c‰¡”ä (‰¡);
	constexpr auto SCREEN_RESOLUTION_X = (7.f / 4.f);
	// c‰¡”ä (c);
	constexpr auto SCREEN_RESOLUTION_Y = (1.f);
	// •ÏŠ·
	const auto ScreenToConsole = Matrix3::CreateScale(Vector2{ SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y });
	// •ÏŠ·
	const auto ConsoleToScreen = ScreenToConsole.Inverse();
	// ƒÎ
	constexpr auto CX_PI_F = static_cast<float>(M_PI);

	// ‹éŒ`•`‰æŠÖ”
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, int FillFlag);

	// ‘È‰~•`‰æŠÖ”
	void DrawOval(Vector2 center, Vector2 size, Color Color, int FillFlag);

	// ‰~•`‰æŠÖ”
	void DrawCircle(Vector2 center, float r, Color Color, int FillFlag);

	// ü•`‰æŠÖ”
	void DrawLine(Vector2 start, Vector2 end, Color Color);

	// ”jü‚ğ•`‰æ
	void DrawDashedLine(Vector2 start, Vector2 end, Color color, float length);

	// ƒEƒBƒ“ƒhƒEƒTƒCƒYæ“¾
	Vector2 GetWindowSize();
}
