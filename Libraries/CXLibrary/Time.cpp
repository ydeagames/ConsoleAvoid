#include "Time.h"

float Time::time;
float Time::deltaTime;

Time::Time(int FPS)
	: FPS(FPS)
	, interval(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)) / FPS)
	, delta_first(std::chrono::nanoseconds::zero())
	, delta_frame(std::chrono::nanoseconds::zero())
	, delta_processing(std::chrono::nanoseconds::zero())
	, timepoint_first(std::chrono::high_resolution_clock::now())
	, timepoint_frame(timepoint_first)
	, timepoint_processing(timepoint_first)
{
	// ����������
	time = 0;
	deltaTime = 0;
}

void Time::Update()
{
	// �����擾
	auto now = std::chrono::high_resolution_clock::now();

	// �����v�Z (�v���I��)
	delta_first = now - timepoint_first;
	delta_frame = now - timepoint_frame;

	// �ŏI�����X�V (�v���J�n)
	timepoint_frame = now;

	// �����ɕϊ�
	time = delta_first.count() * 1e-9f;
	deltaTime = delta_frame.count() * 1e-9f;
}

void Time::WaitFrame()
{
	// �����擾
	auto now = std::chrono::high_resolution_clock::now();

	// �����v�Z (�v���I��)
	delta_processing = now - timepoint_processing;

	// �X���[�v���Ԍv�Z
	auto sleeptime = std::max(std::chrono::nanoseconds::zero(), interval - delta_processing);

	// �X���[�v
	Sleep(static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(sleeptime).count()));

	// �ŏI�����X�V (�v���J�n)
	timepoint_processing = std::chrono::high_resolution_clock::now();
}
