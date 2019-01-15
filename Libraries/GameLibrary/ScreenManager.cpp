#include "ScreenManager.h"

ScreenManager::ScreenManager()
	: context(ScreenContext{ GetStdHandle(STD_OUTPUT_HANDLE) })
{
}

void ScreenManager::UpdateContext()
{
	// �X�N���[���o�b�t�@�Ɋւ�����
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	// �X�N���[���o�b�t�@�Ɋւ�����̎擾
	GetConsoleScreenBufferInfo(context.handle, &screenBufferInfo);

	// �X�N���[���o�b�t�@�C��
	SetConsoleScreenBufferSize(context.handle, COORD{ screenBufferInfo.srWindow.Right + 1, screenBufferInfo.srWindow.Bottom + 1 });

	// �R���e�L�X�g�X�V
	context = ScreenContext{ context.handle, context.default_pixel };
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
	if (newSize.X - context.boundsMax.X > 0 || newSize.Y - context.boundsMax.Y > 0)
	{
		// �g��̏ꍇ
		if (!SetConsoleScreenBufferSize(context.handle, newSize))
			return false;

		if (!SetConsoleWindowInfo(context.handle, true, &newRect))
			return false;
	}
	else
	{
		// �k���̏ꍇ
		if (!SetConsoleWindowInfo(context.handle, true, &newRect))
			return false;

		if (!SetConsoleScreenBufferSize(context.handle, newSize))
			return false;
	}

	// �R���e�L�X�g�X�V
	UpdateContext();

	return true;
}

bool ScreenManager::SetWindowSize(int width, int height)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // �t�H���g���

	// ���ݎg�p���̃t�H���g�̎擾
	GetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// �R���\�[���T�C�Y
	return SetScreenSize(width / fontInfo.dwFontSize.X, height / fontInfo.dwFontSize.Y);
}

void ScreenManager::SetFontSize(int size)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // �t�H���g���

	// ���ݎg�p���̃t�H���g�̎擾
	GetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// �t�H���g�T�C�Y�̐ݒ�
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(size / PixelAspectRatio), static_cast<SHORT>(size) };

	// �t�H���g�̍X�V
	SetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// �R���e�L�X�g�X�V
	UpdateContext();
}

void ScreenManager::SetPixelSize(int size)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };    // �t�H���g���

	// ���ݎg�p���̃t�H���g�̎擾
	GetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// �t�H���g�T�C�Y�̐ݒ�
	COORD oldFontSize = fontInfo.dwFontSize;

	// �t�H���g�T�C�Y�̐ݒ�
	fontInfo.dwFontSize = COORD{ static_cast<SHORT>(size / PixelAspectRatio), static_cast<SHORT>(size) };

	// �t�H���g�̍X�V
	SetCurrentConsoleFontEx(context.handle, false, &fontInfo);

	// �R���\�[���T�C�Y
	SetScreenSize(
		context.boundsMax.X * fontInfo.dwFontSize.X / oldFontSize.X,
		context.boundsMax.Y * fontInfo.dwFontSize.Y / oldFontSize.Y);
}

void ScreenManager::SetDefaultText(WCHAR ch)
{
	auto& pixel = context.default_pixel;
	pixel.Char.UnicodeChar = ch;
}

void ScreenManager::SetDefaultBackground(Color background)
{
	auto& pixel = context.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.back(background);
}

void ScreenManager::SetDefaultForeground(Color foreground)
{
	auto& pixel = context.default_pixel;
	pixel.Attributes = Attributes{ pixel.Attributes }.text(foreground);
}

void ScreenManager::SetBackground(Color color)
{
	// �����ݒ�
	SetConsoleTextAttribute(context.handle, Attributes{ context.attributes }.back(color));

	// �R���e�L�X�g�X�V
	UpdateContext();
}

void ScreenManager::SetForeground(Color color)
{
	// �����ݒ�
	SetConsoleTextAttribute(context.handle, Attributes{ context.attributes }.text(color));

	// �R���e�L�X�g�X�V
	UpdateContext();
}

void ScreenManager::SetCursorVisibility(bool isVisible)
{
	// ���[�J���ϐ��̐錾
	CONSOLE_CURSOR_INFO cursorInfo;    // �J�[�\�����

	// �J�[�\���̕\����Ԃ̕ύX
	GetConsoleCursorInfo(context.handle, &cursorInfo);
	cursorInfo.bVisible = isVisible;
	SetConsoleCursorInfo(context.handle, &cursorInfo);
}

void ScreenManager::SetTitle(LPCWSTR title)
{
	SetConsoleTitleW(title);
}

void ScreenManager::SetCursorPosition(COORD pos)
{
	// �J�[�\���ʒu�̐ݒ�
	SetConsoleCursorPosition(context.handle, pos);
}
