#pragma once
#include "Color.h"
#include "Vector2.h"
#include "Matrix.h"

namespace CXLib
{
	// 縦横比 (横);
	constexpr auto SCREEN_RESOLUTION_X = (7.f / 4.f);
	// 縦横比 (縦);
	constexpr auto SCREEN_RESOLUTION_Y = (1.f);
	// 変換
	const auto ScreenToConsole = Matrix3::CreateScale(Vector2{ SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y });
	// 変換
	const auto ConsoleToScreen = ScreenToConsole.Inverse();
	// π
	constexpr auto CX_PI_F = static_cast<float>(M_PI);

	// ワールド座標をコンソール座標に変換
	float WorldToConsoleXF(float world_x, float size = 1);

	// ワールド座標をコンソール座標に変換
	float WorldToConsoleYF(float world_y, float size = 1);

	// ワールド座標をコンソール座標に変換
	SHORT WorldToConsoleX(float world_x, float size = 1);

	// ワールド座標をコンソール座標に変換
	SHORT WorldToConsoleY(float world_y, float size = 1);

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldXF(float screen_x, float size = 1);

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldYF(float screen_y, float size = 1);

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldX(SHORT screen_x, float size = 1);

	// コンソール座標をワールド座標に変換
	float ConsoleToWorldY(SHORT screen_y, float size = 1);

	// 矩形描画関数
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, int FillFlag);

	// 楕円描画関数
	void DrawOval(Vector2 center, Vector2 size, Color Color, int FillFlag);

	// 円描画関数
	void DrawCircle(Vector2 center, float r, Color Color, int FillFlag);

	// 線描画関数
	void DrawLine(Vector2 start, Vector2 end, Color Color);

	// 破線を描画
	void DrawDashedLine(Vector2 start, Vector2 end, Color color, float length);
}
