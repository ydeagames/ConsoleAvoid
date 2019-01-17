#include "LogoScene.h"

LogoScene::LogoScene()
{
	class LogoRenderer : public Component
	{
		CXImage image;

		void Start()
		{
			image = LoadGraph("Resources/Textures/ydeagames.ppm");
		}

		void Render()
		{
			auto windowsize = GetWindowSize();
			auto imagesize = image.GetSize();
			auto windowaspect = windowsize.y / windowsize.x;
			auto imageaspect = imagesize.y / imagesize.x;
			image.DrawGraph(Matrix3::CreateTranslation(-Vector2::one / 2) * Matrix3::CreateScale(windowsize) * Matrix3::CreateTranslation(windowsize / 2));
		}
	};

	GameObject::Create("LogoRenderer")->AddNewComponent<LogoRenderer>();
}

LogoScene::~LogoScene()
{
}
