#include "ScreenContext.h"

ScreenContext::ScreenContext(const HANDLE& hOut, CHAR_INFO default_pixel)
	: handle(hOut)
	, boundsMin(COORD{ 0, 0 })
	, default_pixel(default_pixel)
{
	// �X�N���[���o�b�t�@�Ɋւ�����
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	// �X�N���[���o�b�t�@�Ɋւ�����̎擾
	GetConsoleScreenBufferInfo(handle, &screenBufferInfo);

	// �R���\�[����ʂɊւ�����̐ݒ�
	handle = handle;
	boundsMax = screenBufferInfo.dwSize;
	screenRegion = { boundsMin.X, boundsMin.Y, boundsMax.X - 1, boundsMax.Y - 1 };
	attributes = screenBufferInfo.wAttributes;

	buffer = std::vector<CHAR_INFO>(boundsMax.X * boundsMax.Y, default_pixel);
	bufferPtr = &buffer[0];
}

// ��ʂ�����������
void ScreenContext::Clear()
{
	// �I�t�X�N���[�����f�t�H���g�s�N�Z���ŏ�����
	int length = static_cast<int>(boundsMax.X * boundsMax.Y);
	std::fill_n(buffer.begin(), length, default_pixel);
}

void ScreenContext::Flush()
{
	WriteConsoleOutputW(handle, bufferPtr, boundsMax, boundsMin, &screenRegion);
}
