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
	FrameTimer m_frame_timer;

public:
	Game();
	~Game();

	void Update();
	void Render();
};
