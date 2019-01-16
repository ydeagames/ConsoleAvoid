#include "CXImage.h"

CXLib::ConsoleImage::ConsoleImage(const std::string & path)
{
}

void CXLib::ConsoleImage::DrawGraph(const Matrix3 & world)
{
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
