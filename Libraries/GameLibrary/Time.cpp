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
	// 小数初期化
	time = 0;
	deltaTime = 0;
}

void Time::Update()
{
	// 時刻取得
	auto now = std::chrono::high_resolution_clock::now();

	// 差を計算 (計測終了)
	delta_first = now - timepoint_first;
	delta_frame = now - timepoint_frame;

	// 最終時刻更新 (計測開始)
	timepoint_frame = now;

	// 小数に変換
	time = delta_first.count() * 1e-9f;
	deltaTime = delta_frame.count() * 1e-9f;
}

void Time::WaitFrame()
{
	// 時刻取得
	auto now = std::chrono::high_resolution_clock::now();

	// 差を計算 (計測終了)
	delta_processing = now - timepoint_processing;

	// スリープ時間計算
	auto sleeptime = std::max(std::chrono::nanoseconds::zero(), interval - delta_processing);

	// スリープ
	Sleep(static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(sleeptime).count()));

	// 最終時刻更新 (計測開始)
	timepoint_processing = std::chrono::high_resolution_clock::now();
}
