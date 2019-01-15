// @file   GameMain.h
//
// @brief  ゲーム関連のヘッダファイル
//
// @date   日付　2019/1/15
//
// @author 制作者名　YdeaGames

#pragma once

// <ゲーム>
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
