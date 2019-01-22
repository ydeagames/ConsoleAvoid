#pragma once

namespace CXLib
{
	// c‰¡”ä (‰¡);
	constexpr auto SCREEN_RESOLUTION_X = (2.f / 1.f);
	// c‰¡”ä (c);
	constexpr auto SCREEN_RESOLUTION_Y = (1.f);
	// •ÏŠ·
	const auto ScreenToConsole = Matrix3::CreateScale(Vector2{ SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y });
	// •ÏŠ·
	const auto ConsoleToScreen = ScreenToConsole.Inverse();
	// ƒÎ
	constexpr auto CX_PI_F = static_cast<float>(M_PI);

	// ‹éŒ`•`‰æŠÖ”
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, bool FillFlag, const Matrix3& world = Matrix3::Identity);

	// ‘È‰~•`‰æŠÖ”
	void DrawOval(Vector2 center, Vector2 size, Color Color, bool FillFlag, const Matrix3& world = Matrix3::Identity);

	// ‰~•`‰æŠÖ”
	void DrawCircle(Vector2 center, float r, Color Color, bool FillFlag, const Matrix3& world = Matrix3::Identity);

	// ü•`‰æŠÖ”
	void DrawLine(Vector2 start, Vector2 end, Color Color, const Matrix3& world = Matrix3::Identity);

	// ”jü‚ğ•`‰æ
	void DrawDashedLine(Vector2 start, Vector2 end, Color color, float length, const Matrix3& world = Matrix3::Identity);

	// ƒEƒBƒ“ƒhƒEƒTƒCƒYæ“¾
	Vector2 GetWindowSize();
}
