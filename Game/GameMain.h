// @file   GameMain.h
//
// @brief  �Q�[���֘A�̃w�b�_�t�@�C��
//
// @date   ���t�@2019/1/15
//
// @author ����Җ��@YdeaGames

#pragma once
#include "FrameTimer.h"
#include "CXFont.h"
#include "Vector2.h"

// <�Q�[��>
class Game
{
private:
	CXLib::CXFont font_pong;
	CXLib::CXFont font;
	Vector2 obj_pos;
	FrameTimer fps;

public:
	Game();
	~Game();

	void Update(void);
	void Render(void);
};
