#include "CXImage.h"

CXLib::CXImage::PixelImage::PixelImage(const std::string & path, ColorCode transparent)
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
		ColorCode color = (red & 0xff) << 16 | (green & 0xff) << 8 | (blue & 0xff) << 0;

		int min_color = -1;
		int min_length = -1;

		if (transparent == Transparent::FirstColor)
			transparent = color;
		if (transparent != Transparent::None && color == transparent)
			min_color = Colors::Transparent;
		else
		{
			for (int i = 0; i < 16; i++)
			{
				ColorCode p_color = Colors::ColorCodes[i];

				int p_red = p_color >> 16 & 0xff;
				int p_green = p_color >> 8 & 0xff;
				int p_blue = p_color >> 0 & 0xff;

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

CXLib::CXImage::CXImage(const std::shared_ptr<PixelImage>& image, int x, int y, int w, int h)
	: image(image)
	, x(x)
	, y(y)
	, w(w)
	, h(h)
{
}

CXLib::CXImage::CXImage(const std::string & path, ColorCode transparent)
	: CXImage(nullptr, 0, 0, 0, 0)
{
	try
	{
		image = std::make_shared<PixelImage>(path, transparent);
		w = image->width;
		h = image->height;
	}
	catch (std::runtime_error e)
	{
	}
}

CXLib::CXImage::CXImage()
	: CXImage(nullptr, 0, 0, 0, 0)
{
}

void CXLib::CXImage::DrawGraph(const Matrix3& world) const
{
	if (image == nullptr)
	{
		DrawBox(Vector2::zero, Vector2::one, Colors::Black, true, world);
		DrawBox(Vector2::zero, Vector2::one / 2, Colors::Magenta, true, world);
		DrawBox(Vector2::one / 2, Vector2::one, Colors::Magenta, true, world);
	}
	else
	{
		Matrix3 matrix = (world * ScreenToConsole);
		// 座標
		Vector2 base_pos = Vector2::zero * matrix;
		// サイズ
		Vector2 base_size = Vector2::one * matrix - base_pos;

		COORD sprite_pos = base_pos;
		COORD sprite_size = base_size;

		// Yループ
		for (SHORT iy = 0; iy < sprite_size.Y; iy++)
		{
			// Xループ
			for (SHORT ix = 0; ix < sprite_size.X; ix++)
			{
				ColorCode color = image->GetPixel(x + static_cast<int>(ix * w / base_size.x), y + static_cast<int>(iy * h / base_size.y));
				// ピクセルが透明でなかったら描画
				if (color != Colors::Transparent)
					Screen::Draw(COORD{ sprite_pos.X + ix, sprite_pos.Y + iy }, color);
			}
		}
	}
}

CXImage CXLib::CXImage::DerivationGraph(Vector2 start, Vector2 size) const
{
	int x1 = x + start.X();
	int y1 = y + start.Y();
	int w1 = std::min(size.X(), w - x1);
	int h1 = std::min(size.Y(), h - y1);
	return { image, x1, y1, w1, h1 };
}

CXImage CXLib::CXImage::DerivationGraphUV(Vector2 startUV, Vector2 sizeUV) const
{
	return DerivationGraph(startUV * Vector2{ w, h }, sizeUV * Vector2{ w, h });
}

CXImage CXLib::LoadGraph(const std::string& path, ColorCode transparent)
{
	return { path, transparent };
}

void CXLib::DrawGraph(Vector2 pos, const CXImage& image, const Matrix3& world)
{
	image.DrawGraph(Matrix3::CreateTranslation(pos) * world);
}

void CXLib::DrawExtendGraph(Vector2 start, Vector2 end, const CXImage& image, const Matrix3& world)
{
	image.DrawGraph(Matrix3::CreateTranslation(start) * Matrix3::CreateScale(end - start) * world);
}
