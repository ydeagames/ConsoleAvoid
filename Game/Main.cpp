#include "GameMain.h"

// 1�t���[��
static int ProcessMessage(void)
{
	static auto& contextIn = ScreenManager::GetInstance().GetInputContext();

	auto& time = Time::GetInstance();
	time.Update();
	time.WaitFrame();
	contextIn.HandleInput();
	InputManager::GetInstance().Update();
	return !SystemUtils::running;
}

// �v���O�����̃G���g���[�|�C���g
int main(void)
{
#if defined(_DEBUG)
	// �������[���[�N���o�@�\�̃Z�b�g�A�b�v
	//SetUpMemoryLeakDetector();
#endif

	static auto& context = ScreenManager::GetInstance().GetOutputContext();

	// ������Ԃ̉�ʃ��[�h�̐ݒ�

	ScreenManager::GetInstance().EnableMouseInput();
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
