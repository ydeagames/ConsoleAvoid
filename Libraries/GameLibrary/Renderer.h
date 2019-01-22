#pragma once
#include "Component.h"
#include "Texture.h"

class Material final
{
public:
	Color base_color;
	Color edge_color;

public:
	Material()
		: base_color(Colors::Transparent)
		, edge_color(Colors::Transparent) {}

public:
	Material& SetBase(Color color);
	Material& SetBorder(Color color);
};

class Renderer : public Component
{
public:
	Material material;

public:
	Renderer()
		: material(material = Material{}.SetBase(Colors::White)) {}
	virtual ~Renderer() {}

public:
	void Render() override = 0;
};

class BoxRenderer : public Renderer
{
public:
	void Render() override;
};

class CircleRenderer : public Renderer
{
public:
	void Render() override;
};

class LineRenderer : public Renderer
{
public:
	void Render() override;
};

class FontTextRenderer : public Component
{
public:
	std::wstring text;
	CXFont font;

public:
	FontTextRenderer(const CXFont& font, const std::wstring& text);

	void Render() override;
};

class TextureRenderer : public Component
{
public:
	Texture texture;

public:
	TextureRenderer(const Texture& image);

	void Render() override;
};

