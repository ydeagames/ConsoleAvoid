#pragma once

// <�t���[���^�C�}�[>
class FrameTimer final
{
private:
	// �t���[�����[�g[fps]
	float m_frame_rate;
	// �t���[�����[�g�v���J�n����
	float m_start_time;
	// �t���[����
	int m_frame_count;

public:
	FrameTimer();

	// ���Z�b�g
	void Reset();
	// �X�V
	void Update();
	// �t���[�����[�g�̎擾
	float GetFrameRate(void);
};
