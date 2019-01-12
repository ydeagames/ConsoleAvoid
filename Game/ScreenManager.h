#pragma once
#include "Singleton.h"
#include "BufferedConsole.h"

class ScreenManager : public Singleton<ScreenManager>
{
private:
	ScreenContext context;

private:
	ScreenManager();
	friend class Singleton<ScreenManager>;

public:
	ScreenContext& GetContext();
};