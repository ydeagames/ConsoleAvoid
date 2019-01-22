#pragma once

namespace CXLib
{
	// �c���� (��);
	constexpr auto SCREEN_RESOLUTION_X = (2.f / 1.f);
	// �c���� (�c);
	constexpr auto SCREEN_RESOLUTION_Y = (1.f);
	// �ϊ�
	const auto ScreenToConsole = Matrix3::CreateScale(Vector2{ SCREEN_RESOLUTION_X, SCREEN_RESOLUTION_Y });
	// �ϊ�
	const auto ConsoleToScreen = ScreenToConsole.Inverse();
	// ��
	constexpr auto CX_PI_F = static_cast<float>(M_PI);

	// ��`�`��֐�
	void DrawBox(Vector2 boundsMin, Vector2 boundsMax, Color Color, bool FillFlag, const Matrix3& world = Matrix3::Identity);

	// �ȉ~�`��֐�
	void DrawOval(Vector2 center, Vector2 size, Color Color, bool FillFlag, const Matrix3& world = Matrix3::Identity);

	// �~�`��֐�
	void DrawCircle(Vector2 center, float r, Color Color, bool FillFlag, const Matrix3& world = Matrix3::Identity);

	// ���`��֐�
	void DrawLine(Vector2 start, Vector2 end, Color Color, const Matrix3& world = Matrix3::Identity);

	// �j����`��
	void DrawDashedLine(Vector2 start, Vector2 end, Color color, float length, const Matrix3& world = Matrix3::Identity);

	// �E�B���h�E�T�C�Y�擾
	Vector2 GetWindowSize();
}
