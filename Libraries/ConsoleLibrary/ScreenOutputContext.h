#pragma once
#include "Color.h"

class ScreenOutputContext
{
public:
	HANDLE handle;				  			// �o�̓f�o�C�X�̃n���h��
	COORD boundsMin;						// �o�b�t�@�[�̃I�t�Z�b�g
	COORD boundsMax;			  			// �o�b�t�@�T�C�Y
	WORD attributes;			  			// ����
	CHAR_INFO default_pixel;	  			// �f�t�H���g�s�N�Z��
	SHORT fontSize;							// �t�H���g�T�C�Y

private:
	SMALL_RECT screenRegion;	  			// �`��̈�
	std::vector<CHAR_INFO> dynamicBuffer;	// �o�b�t�@
	std::vector<CHAR_INFO> staticBuffer;	// �ÓI�o�b�t�@
	bool useStaticBuffer;					// �ÓI�o�b�t�@
	CHAR_INFO* currentBuffer;		  		// �J�����g�o�b�t�@�|�C���^

public:
	ScreenOutputContext(const HANDLE& handle, CHAR_INFO default_pixel = { ' ', Attributes{ Colors::Black, Colors::White } });

public:
	void Clear();
	void Flush();

public:
	bool HasStaticBuffer();
	void ClearStaticBuffer();
	void BeginStaticBuffer();
	void EndStaticBuffer();
	bool IsStaticBufferMode();

public:
	inline CHAR_INFO& Pixel(COORD coord) { return currentBuffer[coord.X + coord.Y * boundsMax.X]; }
};
