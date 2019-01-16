#pragma once
#include "CXLib.h"

namespace CXLib
{
	class ConsoleImage
	{
	public:
		int width, height;
		std::vector<Color> image;

	public:
		ConsoleImage(const std::string& path);

	public:
		void DrawGraph(const Matrix3& world = Matrix3::Identity);
		//ConsoleImage DerivationGraph(Vector2 start, Vector2 size);
	};
	using CXImage = std::shared_ptr<ConsoleImage>;

	// イメージハンドルを作成する
	CXImage LoadGraph(const std::string& path);

	// イメージ描画関数
	void DrawGraph(Vector2 pos, const CXImage& image, const Matrix3& world = Matrix3::Identity);
	void DrawExtendGraph(Vector2 start, Vector2 end, const CXImage& image, const Matrix3& world = Matrix3::Identity);
}