#include "SystemUtils.h"

bool SystemUtils::running = true;

// �Q�[���̏I��
void SystemUtils::ExitGame(void)
{
	running = false;
}
