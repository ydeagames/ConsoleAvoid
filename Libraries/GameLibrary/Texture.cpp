#include "Texture.h"

Texture::Texture()
	: Texture(CXImage{})
{
}

Texture::Texture(const CXImage & image)
	: Texture(std::vector<CXImage>{ image }, 1)
{
}

Texture::Texture(const std::vector<CXImage>& images, float interval, bool loop_enable)
	: images(images)
	, interval(interval)
	, loop_enable(loop_enable)
	, firstTime(Time::time)
{
}

const CXImage& Texture::GetFrame()
{
	float time = Time::time - firstTime;
	int frame = MathUtils::Loop(static_cast<int>(time / interval), images.size());
	return images[frame];
}
