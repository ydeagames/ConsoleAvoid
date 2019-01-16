#include "CXImage.h"

CXLib::ConsoleImage::PixelImage::PixelImage(const std::string & path, ColorCode transparent)
{
	std::vector<uint8_t> pixels = std::vector<uint8_t>();
	size_t img_width, img_height;

	thinks::ppm::readRgbImage(path, &img_width, &img_height, &pixels);

	width = static_cast<int>(img_width);
	height = static_cast<int>(img_height);
	data.reserve(img_width*img_height);

	for (auto itr = pixels.begin(); itr != pixels.end();)
	{
		uint8_t red = *(itr++);
		uint8_t green = *(itr++);
		uint8_t blue = *(itr++);

		int min_color = -1;
		int min_length = -1;
		if (((red & 0xff) << 16 | (green & 0xff) << 8 | (blue & 0xff) << 0) == transparent)
			min_color = Colors::Transparent;
		else
		{
			for (int i = 0; i < 16; i++)
			{
				ColorCode color = Colors::ColorCodes[i];

				int p_red = color >> 16 & 0xff;
				int p_green = color >> 8 & 0xff;
				int p_blue = color >> 0 & 0xff;

				int length = (p_red - red)*(p_red - red) + (p_green - green)*(p_green - green) + (p_blue - blue)*(p_blue - blue);

				if (min_color < 0 || length < min_length)
				{
					min_color = i;
					min_length = length;
				}
			}
		}

		data.push_back(min_color);
	}

	dataPtr = &data[0];
}

CXLib::ConsoleImage::ConsoleImage(const std::string & path, ColorCode transparent)
	: image(std::make_shared<PixelImage>(path, transparent))
{
}

void CXLib::ConsoleImage::DrawGraph(const Matrix3 & world)
{
	Matrix3 matrix = (world * ScreenToConsole);
	// 座標
	Vector2 base_pos = Vector2::zero * matrix;
	// サイズ
	Vector2 base_size = Vector2::one * matrix - base_pos;

	COORD sprite_pos = base_pos;
	COORD sprite_size = Vector2{ w, h }*base_size;

	// Yループ
	for (SHORT iy = 0; iy < sprite_size.Y; iy++)
	{
		// Xループ
		for (SHORT ix = 0; ix < sprite_size.X; ix++)
		{
			ColorCode color = image->GetPixel(x + static_cast<int>(ix / base_size.x), y + static_cast<int>(iy / base_size.y));
			// ピクセルが透明でなかったら描画
			if (color == Colors::Transparent)
				Screen::Draw(COORD{ sprite_pos.X + ix, sprite_pos.Y + iy }, color);
		}
	}
}

CXImage CXLib::LoadGraph(const std::string & path)
{
	return std::make_shared<ConsoleImage>(path);
}

void CXLib::DrawGraph(Vector2 pos, const CXImage & image, const Matrix3 & world)
{
	image->DrawGraph(Matrix3::CreateTranslation(pos) * world);
}

void CXLib::DrawExtendGraph(Vector2 start, Vector2 end, const CXImage & image, const Matrix3 & world)
{
	image->DrawGraph(Matrix3::CreateTranslation(start) * Matrix3::CreateScale(end - start) * world);
}
