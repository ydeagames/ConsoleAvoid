#pragma once
#include "Color.h"

class ScreenContext
{
public:
	HANDLE handle;				  	// �o�̓f�o�C�X�̃n���h��
	COORD boundsMin;				// �o�b�t�@�[�̃I�t�Z�b�g
	COORD boundsMax;			  	// �o�b�t�@�T�C�Y
	WORD attributes;			  	// ����
	CHAR_INFO default_pixel;	  	// �f�t�H���g�s�N�Z��

private:
	SMALL_RECT screenRegion;	  	// �`��̈�
	std::vector<CHAR_INFO> buffer;	// �o�b�t�@
	CHAR_INFO* bufferPtr;		  	// �o�b�t�@

public:
	ScreenContext(const HANDLE& handle, CHAR_INFO default_pixel = { ' ', Attributes{ Colors::Black, Colors::White } });

public:
	void Clear();
	void Flush();

public:
	inline CHAR_INFO& Pixel(COORD coord) { return bufferPtr[coord.X + coord.Y * boundsMax.X]; }
};
