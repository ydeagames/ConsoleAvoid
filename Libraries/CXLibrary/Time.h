#pragma once

class Time : public Singleton<Time>
{
public:
	// ����t���[������̌o�ߎ���(�b)
	static float time;
	// �O�̃t���[������̌o�ߎ���(�b)
	static float deltaTime;
	//�X�s�[�h
	static float speed;

public:
	// FPS
	const int FPS;
	// FPS���狁�߂�1�t���[���̎���
	const std::chrono::nanoseconds interval;

public:
	// ����̎���
	std::chrono::time_point<std::chrono::high_resolution_clock> timepoint_first;
	// �t���[���Ԋu�p�̎���
	std::chrono::time_point<std::chrono::high_resolution_clock> timepoint_frame;
	// �������ԗp�̎���
	std::chrono::time_point<std::chrono::high_resolution_clock> timepoint_processing;
	// ���񂩂�̊Ԋu
	std::chrono::nanoseconds delta_first;
	// 1�t���[���̊Ԋu
	std::chrono::nanoseconds delta_frame;
	// 1�t���[���̏�������
	std::chrono::nanoseconds delta_processing;

private:
	Time(int FPS = 60);
	friend class Singleton<Time>;

public:
	void Update();
	void WaitFrame();
};
