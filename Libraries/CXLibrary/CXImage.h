#pragma once
#include "CXLib.h"

namespace CXLib
{
	class ConsoleImage
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
			constexpr Color GetPixel(int x, int y) const { return dataPtr[x + y * width]; }

		public:
			PixelImage(const std::string& path, ColorCode transparent);
		};

	private:
		std::shared_ptr<PixelImage> image;
		int x, y, w, h;

	public:
		ConsoleImage(const std::string& path, ColorCode transparent = 0x0000ff);

	public:
		void DrawGraph(const Matrix3& world = Matrix3::Identity);
		//ConsoleImage DerivationGraph(Vector2 start, Vector2 size);
	};
	using CXImage = std::shared_ptr<ConsoleImage>;

	// �C���[�W�n���h�����쐬����
	CXImage LoadGraph(const std::string& path);

	// �C���[�W�`��֐�
	void DrawGraph(Vector2 pos, const CXImage& image, const Matrix3& world = Matrix3::Identity);
	void DrawExtendGraph(Vector2 start, Vector2 end, const CXImage& image, const Matrix3& world = Matrix3::Identity);
}