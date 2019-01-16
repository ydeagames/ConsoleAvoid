#pragma once
#include "CXLib.h"

namespace CXLib
{
	// �t�H���g�̎��
	typedef enum {
		CXFONT_DEFAULT,
		CXFONT_PONG
	} CXFontType;

	// �t�H���g
	typedef struct {
		CXFontType type;
		float size;
	} CXFont;

	// �t�H���g�n���h�����쐬����
	CXFont CreateFontToHandle(CXFontType Font, int Size);

	// ������`��֐�
	void DrawString(Vector2 pos, LPCWSTR String, Color Color);

	// �t�H���g�n���h�����g�p����������̕`�敝(���[���h���W)���擾����
	float GetDrawStringWidthToHandle(LPCWSTR String, const CXFont* FontHandle);

	// �t�H���g�n���h�����g�p���ĕ������`�悷��
	void DrawStringToHandle(Vector2 pos, LPCWSTR String, Color Color, const CXFont* FontHandle);
}