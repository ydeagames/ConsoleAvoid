#pragma once

class Texture
{
public:
	std::vector<CXImage> images;
	float interval;
	bool loop_enable;

private:
	float firstTime;

public:
	Texture();
	Texture(const CXImage& image);
	Texture(const std::vector<CXImage>& images, float interval, bool loop_enable = true);

public:
	const CXImage& GetFrame();
};

