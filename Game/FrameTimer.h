#pragma once

// <フレームタイマー>
class FrameTimer final
{
private:
	// フレームレート[fps]
	float m_frame_rate;
	// フレームレート計測開始時間
	float m_start_time;
	// フレーム数
	int m_frame_count;

public:
	FrameTimer();

	// リセット
	void Reset();
	// 更新
	void Update();
	// フレームレートの取得
	float GetFrameRate(void);
};
