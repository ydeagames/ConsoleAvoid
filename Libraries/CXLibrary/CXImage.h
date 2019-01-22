#pragma once
#include "CXLib.h"

namespace CXLib
{
	namespace Transparent
	{
		enum : ColorCode
		{
			None = -2,
			FirstColor = -3,
		};
	}

	class CXImage
	{
	public:
		class PixelImage
		{
		public:
			int width, height;

		private:
			std::vector<Color> data;
			const Color* dataPtr;

		public:
			inline Color GetPixel(int x, int y) const { return dataPtr[x + y * width]; }

		public:
			PixelImage(const std::string& path, ColorCode transparent);
		};

	private:
		std::shared_ptr<PixelImage> image;
		int x, y, w, h;

	public:
		inline Vector2 GetSize() const { return{ w, h }; }

	private:
		CXImage(const std::shared_ptr<PixelImage>& image, int x, int y, int w, int h);

	public:
		CXImage(const std::string& path, ColorCode transparent = 0x0000ff);
		CXImage();

	public:
		void DrawGraph(const Matrix3& world = Matrix3::Identity) const;

	public:
		CXImage DerivationGraph(Vector2 start, Vector2 size) const;
		CXImage DerivationGraphUV(Vector2 startUV, Vector2 sizeUV) const;
	};

	// イメージハンドルを作成する
	CXImage LoadGraph(const std::string& path, ColorCode transparent = 0x0000ff);

	// イメージ描画関数
	void DrawGraph(const CXImage& image, const Matrix3& world = Matrix3::Identity);
}