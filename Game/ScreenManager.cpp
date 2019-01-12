#include "ScreenManager.h"

ScreenManager::ScreenManager()
	: context(ScreenContext{ GetStdHandle(STD_OUTPUT_HANDLE) })
{
}

ScreenContext& ScreenManager::GetContext()
{
	return context;
}
