#pragma once
#include "Color.h"
#include "Vector2.h"
#include "Matrix.h"

namespace CXLib
{
	// �c���� (��);
	constexpr auto SCREEN_RESOLUTION_X = (7.f / 4.f);
	// �c���� (�c);
	constexpr auto SCREEN_RESOLUTION_Y = (1.f);
	// �ϊ�
	const auto ScreenToConsole = Matrix3::CreateScale(Vector2{ SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y });
	// �ϊ�
	const auto ConsoleToScreen = ScreenToConsole.Inverse();
	// ��
	constexpr auto CX_PI_F = static_cast<float>(M_PI);

	// ��`�`��֐�
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, int FillFlag);

	// �ȉ~�`��֐�
	void DrawOval(Vector2 center, Vector2 size, Color Color, int FillFlag);

	// �~�`��֐�
	void DrawCircle(Vector2 center, float r, Color Color, int FillFlag);

	// ���`��֐�
	void DrawLine(Vector2 start, Vector2 end, Color Color);

	// �j����`��
	void DrawDashedLine(Vector2 start, Vector2 end, Color color, float length);

	// �E�B���h�E�T�C�Y�擾
	Vector2 GetWindowSize();
}
