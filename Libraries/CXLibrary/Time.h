#pragma once

class Time : public Singleton<Time>
{
public:
	// 初回フレームからの経過時間(秒)
	static float time;
	// 前のフレームからの経過時間(秒)
	static float deltaTime;
	//スピード
	static float speed;

public:
	// FPS
	const int FPS;
	// FPSから求めた1フレームの時間
	const std::chrono::nanoseconds interval;

public:
	// 初回の時間
	std::chrono::time_point<std::chrono::high_resolution_clock> timepoint_first;
	// フレーム間隔用の時刻
	std::chrono::time_point<std::chrono::high_resolution_clock> timepoint_frame;
	// 処理時間用の時刻
	std::chrono::time_point<std::chrono::high_resolution_clock> timepoint_processing;
	// 初回からの間隔
	std::chrono::nanoseconds delta_first;
	// 1フレームの間隔
	std::chrono::nanoseconds delta_frame;
	// 1フレームの処理時間
	std::chrono::nanoseconds delta_processing;

private:
	Time(int FPS = 60);
	friend class Singleton<Time>;

public:
	void Update();
	void WaitFrame();
};
