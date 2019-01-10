#include "MathUtils.h"

namespace MathUtils
{
	std::random_device rnd;		// 非決定的な乱数生成器
	std::mt19937 mt(rnd());		// メルセンヌ・ツイスタの32ビット版、引数は初期シード
									   
	// 0.0≦x≦1.0 の小数ランダム生成
	float GetRandScale()
	{
		return static_cast<float>(GetRand(RAND_MAX)) / RAND_MAX;
	}

	// 0.0≦x≦max の整数ランダム生成
	int GetRand(int max)
	{
		return mt() % (max + 1);
	}

	// 0.0≦x≦max の小数ランダム生成
	float GetRand(float max)
	{
		return GetRandScale() * max;
	}

	// min≦x≦max の整数ランダム生成
	int GetRandRange(int min, int max)
	{
		if (max < min)
			std::swap(min, max);
		return GetRand(max - min) + min;
	}

	// min≦x≦max の小数ランダム生成
	float GetRandRange(float min, float max)
	{
		if (max < min)
			std::swap(min, max);
		return GetRand(max - min) + min;
	}

	// 0～w の整数ループ
	int GetLoop(int x, int w)
	{
		return ((x % w) + w) % w;
	}

	// 0～w の小数ループ
	float GetLoop(float x, float w)
	{
		return std::fmodf((std::fmodf(x, w) + w), w);
	}

	// min～max の整数ループ
	int GetLoopRange(int x, int min, int max)
	{
		if (max < min)
			std::swap(min, max);
		return GetLoop(x - min, max - min) + min;
	}

	// min～max の小数ループ
	float GetLoopRange(float x, float min, float max)
	{
		if (max < min)
			std::swap(min, max);
		return GetLoop(x - min, max - min) + min;
	}

	// 浮動小数点数型の誤差を考慮に入れた比較
	bool FloatEquals(float a, float b)
	{
		return std::abs(a - b) <= std::numeric_limits<float>::epsilon();
	}

	// 角度の単位変換([度]→[ラジアン])
	float ToRadians(float degree)
	{
		return degree * static_cast<float>(M_PI) / 180.0f;
	}

	// 角度の単位変換([ラジアン]→[度])
	float ToDegrees(float radian)
	{
		return radian * 180.0f / static_cast<float>(M_PI);
	}

	// 0～maxの範囲でxは何%の位置にいるのか
	// ※ 範囲外は範囲内に収められます
	float GetPercentage(float x, float max)
	{
		float n = GetClamp(x, 0.f, max);
		return n / max;
	}

	// min～maxの範囲でxは何%の位置にいるのか
	// ※ 範囲外は範囲内に収められます
	float GetPercentageRange(float x, float min, float max)
	{
		if (max < min)
		{
			x = max - x + min;
			std::swap(min, max);
		}
		return GetPercentage(x - min, max - min);
	}

	// 0～maxの範囲でのpercent%の位置
	float GetPercentValue(float percent, float max)
	{
		return max * percent;
	}

	// min～maxの範囲でのpercent%の位置
	float GetPercentValueRange(float percent, float min, float max)
	{
		if (max < min)
		{
			percent = 1 - percent;
			std::swap(min, max);
		}
		return GetPercentValue(percent, max - min) + min;
	}
}