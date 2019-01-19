#include "ScreenManager.h"

ScreenManager::ScreenManager()
	: inputContext(ScreenInputContext{ GetStdHandle(STD_INPUT_HANDLE) })
	, outputContext(ScreenOutputContext{ GetStdHandle(STD_OUTPUT_HANDLE) })
{
}

void ScreenManager::UpdateOutputContext()
{
	// �X�N���[���o�b�t�@�Ɋւ�����
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	// �X�N���[���o�b�t�@�Ɋւ�����̎擾
	GetConsoleScreenBufferInfo(outputContext.handle, &screenBufferInfo);

	// �X�N���[���o�b�t�@�C��
	SetConsoleScreenBufferSize(outputContext.handle, COORD{ screenBufferInfo.srWindow.Right + 1, screenBufferInfo.srWindow.Bottom + 1 });

	// �R���e�L�X�g�X�V
	outputContext = ScreenOutputContext{ outputContext.handle, outputContext.default_pixel };
}

bool ScreenManager::SetScreenSize(int width, int height)
{
	// �X�N���[���o�b�t�@�̃T�C�Y
	COORD  newSize = COORD{ static_cast<SHORT>(width), static_cast<SHORT>(height) };
	// �E�B���h�E��`
	SMALL_RECT newRect =
	{
		0,				// ��
		0,				// ��
		newSize.X - 1,	// �E
		newSize.Y - 1	// ��
	};

	// ��ʂ̃T�C�Y�ύX
	if (newSize.X - outputContext.boundsMax.X > 0 || newSize.Y - outputContext.boundsMax.Y > 0)
	{
		// �g��̏ꍇ
		if (!SetConsoleScreenBufferSize(outputContext.handle, newSize))
			return false;

		if (!SetConsoleWindowInfo(outputContext.handle, true, &newRect))
			return false;
	}
	else
	{
		// �k���̏ꍇ
		if (!SetConsoleWindowInfo(outputContext.handle, true, &newRect))
			return false;

		if (!SetConsoleScreenBufferSize(outputContext.handle, newSize))
			return false;
	}

	// �R���e�L�X�g�X�V
	UpdateOutputContext();

	return true;
}

bool ScreenManager::SetWindowSize(int width, int height)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // �t�H���g���

	// ���ݎg�p���̃t�H���g�̎擾
	GetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// �R���\�[���T�C�Y
	return SetScreenSize(width / fontInfo.dwFontSize.X, height / fontInfo.dwFontSize.Y);
}

void ScreenManager::SetFontSize(int size)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // �t�H���g���

	// ���ݎg�p���̃t�H���g�̎擾
	GetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// �t�H���g�T�C�Y�̐ݒ�
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(size / PixelAspectRatio), static_cast<SHORT>(size) };

	// �t�H���g�̍X�V
	SetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// �R���e�L�X�g�X�V
	UpdateOutputContext();
}

void ScreenManager::SetPixelSize(int size)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // �t�H���g���

	// ���ݎg�p���̃t�H���g�̎擾
	GetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// �t�H���g�T�C�Y�̐ݒ�
	COORD oldFontSize = fontInfo.dwFontSize;

	// �t�H���g�T�C�Y�̐ݒ�
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(size / PixelAspectRatio), static_cast<SHORT>(size) };

	// �t�H���g�̍X�V
	SetCurrentConsoleFontEx(outputContext.handle, false, &fontInfo);

	// �R���\�[���T�C�Y
	SetScreenSize(
		outputContext.boundsMax.X * fontInfo.dwFontSize.X / oldFontSize.X,
		outputContext.boundsMax.Y * fontInfo.dwFontSize.Y / oldFontSize.Y);
}

void ScreenManager::SetDefaultText(WCHAR ch)
{
	auto& pixel = outputContext.default_pixel;
	pixel.Char.UnicodeChar = ch;
}

void ScreenManager::SetDefaultBackground(Color background)
{
	auto& pixel = outputContext.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.back(background);
}

void ScreenManager::SetDefaultForeground(Color foreground)
{
	auto& pixel = outputContext.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.text(foreground);
}

void ScreenManager::SetBackground(Color color)
{
	// �����ݒ�
	SetConsoleTextAttribute(outputContext.handle, Attributes{ outputContext.attributes }.back(color));

	// �R���e�L�X�g�X�V
	UpdateOutputContext();
}

void ScreenManager::SetForeground(Color color)
{
	// �����ݒ�
	SetConsoleTextAttribute(outputContext.handle, Attributes{ outputContext.attributes }.text(color));

	// �R���e�L�X�g�X�V
	UpdateOutputContext();
}

void ScreenManager::SetCursorVisibility(bool isVisible)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_CURSOR_INFO cursorInfo;    // �J�[�\�����

	// �J�[�\���̕\����Ԃ̕ύX
	GetConsoleCursorInfo(outputContext.handle, &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(outputContext.handle, &cursorInfo);
}

void ScreenManager::SetTitle(LPCWSTR title)
{
	SetConsoleTitleW(title);
}

void ScreenManager::SetCursorPosition(COORD pos)
{
	// �J�[�\���ʒu�̐ݒ�
	SetConsoleCursorPosition(outputContext.handle, pos);
}

void ScreenManager::EnableMouseInput()
{
	// �ȈՕҏW���[�h�𖳌���
	SetConsoleMode(inputContext.handle, ENABLE_EXTENDED_FLAGS);
	// �}�E�X�L����
	SetConsoleMode(inputContext.handle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}
