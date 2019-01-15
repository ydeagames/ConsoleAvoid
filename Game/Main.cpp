#include "GameMain.h"
#include "Screen.h"
#include "ScreenManager.h"
#include "Input.h"
#include "Time.h"
#include "SystemUtils.h"

// 1�t���[��
static int ProcessMessage(void)
{
	auto& time = Time::GetInstance();
	time.Update();
	time.WaitFrame();
	InputManager::GetInstance().Update();
	return !SystemUtils::running;
}

// �v���O�����̃G���g���[�|�C���g
int main(void)
{
	static auto& context = ScreenManager::GetInstance().GetContext();

	// ������Ԃ̉�ʃ��[�h�̐ݒ�

	ScreenManager::GetInstance().SetFontSize(7);
	ScreenManager::GetInstance().SetWindowSize(640, 480);
	ScreenManager::GetInstance().SetCursorVisibility(false);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	// �Q�[���̏���

	// �Q�[���̏���������
	Game* game = new Game;

	while (!ProcessMessage())
	{
		// �Q�[���̍X�V����
		game->Update();
		// �Q�[���̕`�揈��
		game->Render();

		// ����ʂ̓��e��\��ʂɔ��f
		context.Flush();
		// ����ʂ̏���
		context.Clear();
	}

	// �Q�[���̏I������
	delete game;

	return 0;
}
