#include "FrameTimer.h"
#include "Time.h"

// <FrameTimerクラス>
FrameTimer::FrameTimer()
	: m_frame_rate(0.f)
	, m_start_time(Time::time)
	, m_frame_count(0)
{
}

// フレームタイマーの更新
void FrameTimer::Update()
{
	// 1秒毎にフレームレートの更新
	m_frame_count++;

	float delta = Time::time - m_start_time;
	if (delta > 1)
	{
		m_frame_rate = m_frame_count / delta;
		m_start_time = Time::time;
		m_frame_count = 0;
	}
}

// フレームタイマーのリセット
void FrameTimer::Reset()
{
	m_start_time = Time::time;
	m_frame_count = 0;
}

// フレームレートの取得
float FrameTimer::GetFrameRate(void)
{
	return m_frame_rate;
}
