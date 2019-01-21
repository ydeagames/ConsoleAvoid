#include "Renderer.h"
#include "Shape.h"

Material& Material::SetBase(Color color, float transparency)
{
	base_color = color;
	base_transparency = transparency;
	return *this;
}

Material& Material::SetBorder(Color color, float transparency, float thickness)
{
	edge_color = color;
	edge_transparency = transparency;
	edge_thickness = thickness;
	return *this;
}

void BoxRenderer::Render()
{
	auto t = gameObject()->transform();
	Box box = Box{ Vector2{}, t->scale, 0 }.Transformed(*t);

	//if (material.base_transparency > 0)
	//{
	//	if (material.base_transparency < 1)
	//	{
	//		int mode, alpha;
	//		GetDrawBlendMode(&mode, &alpha);
	//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(material.base_transparency * 255));
	//		Graphics::DrawBoxRotate(box, material.base_color, true);
	//		SetDrawBlendMode(mode, alpha);
	//	}
	//	else
	//		Graphics::DrawBoxRotate(box, material.base_color, true);
	//}
	//if (material.edge_transparency > 0)
	//{
	//	if (material.edge_transparency < 1)
	//	{
	//		int mode, alpha;
	//		GetDrawBlendMode(&mode, &alpha);
	//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(material.edge_transparency * 255));
	//		Graphics::DrawBoxRotate(box, material.edge_color, false, material.edge_thickness);
	//		SetDrawBlendMode(mode, alpha);
	//	}
	//	else
	//		Graphics::DrawBoxRotate(box, material.edge_color, false, material.edge_thickness);
	//}
}

void CircleRenderer::Render()
{
	auto t = gameObject()->transform();
	Bounds box = Bounds{ Vector2{}, Vector2{ 1, 1 } }.Transformed(*t);

	//if (material.base_transparency > 0)
	//{
	//	if (material.base_transparency < 1)
	//	{
	//		int mode, alpha;
	//		GetDrawBlendMode(&mode, &alpha);
	//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(material.base_transparency * 255));
	//		DrawOvalAA(box.GetX(HorizontalSide::LEFT), box.GetY(VerticalSide::TOP), box.GetExtents().x, box.GetExtents().y, 32, material.base_color, true);
	//		SetDrawBlendMode(mode, alpha);
	//	}
	//	else
	//		DrawOvalAA(box.GetX(HorizontalSide::LEFT), box.GetY(VerticalSide::TOP), box.GetExtents().x, box.GetExtents().y, 32, material.base_color, true);
	//}
	//if (material.edge_transparency > 0)
	//{
	//	if (material.edge_transparency < 1)
	//	{
	//		int mode, alpha;
	//		GetDrawBlendMode(&mode, &alpha);
	//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(material.edge_transparency * 255));
	//		DrawOvalAA(box.GetX(HorizontalSide::LEFT), box.GetY(VerticalSide::TOP), box.GetExtents().x, box.GetExtents().y, 32, material.edge_color, false, material.edge_thickness);
	//		SetDrawBlendMode(mode, alpha);
	//	}
	//	else
	//		DrawOvalAA(box.GetX(HorizontalSide::LEFT), box.GetY(VerticalSide::TOP), box.GetExtents().x, box.GetExtents().y, 32, material.edge_color, false, material.edge_thickness);
	//}
}

void LineRenderer::Render()
{
	auto line = std::dynamic_pointer_cast<LineCollider>(gameObject()->GetComponent<Collider>())->GetShape(*gameObject()->transform());
	//DrawLineAA(line.p1.x, line.p1.y, line.p2.x, line.p2.y, material.edge_color, material.edge_thickness);
}

FontTextRenderer::FontTextRenderer(const CXFont& font, const std::wstring& text)
	: text(text)
	, font(font)
{
}

void FontTextRenderer::Render()
{
	auto world = gameObject()->transform()->GetMatrix();
	DrawStringToHandle(text.c_str(), Colors::White, font, world);
}

TextureRenderer::TextureRenderer(const Texture & texture)
	: texture(texture)
{
}

void TextureRenderer::Render()
{
	auto world = gameObject()->transform()->GetMatrix();
	texture.GetFrame().DrawGraph(world);
}
