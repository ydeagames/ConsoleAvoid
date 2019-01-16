#include "FrameTimer.h"
#include "Time.h"

// <FrameTimer�N���X>
FrameTimer::FrameTimer()
	: m_frame_rate(0.f)
	, m_start_time(Time::time)
	, m_frame_count(0)
{
}

// �t���[���^�C�}�[�̍X�V
void FrameTimer::Update()
{
	// 1�b���Ƀt���[�����[�g�̍X�V
	m_frame_count++;

	float delta = Time::time - m_start_time;
	if (delta > 1)
	{
		m_frame_rate = m_frame_count / delta;
		m_start_time = Time::time;
		m_frame_count = 0;
	}
}

// �t���[���^�C�}�[�̃��Z�b�g
void FrameTimer::Reset()
{
	m_start_time = Time::time;
	m_frame_count = 0;
}

// �t���[�����[�g�̎擾
float FrameTimer::GetFrameRate(void)
{
	return m_frame_rate;
}
