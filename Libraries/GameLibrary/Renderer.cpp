#include "Renderer.h"
#include "Shape.h"
#include "Transform.h"
#include "GameObject.h"
#include "Collider.h"

Material& Material::SetBase(Color color)
{
	base_color = color;
	return *this;
}

Material& Material::SetBorder(Color color)
{
	edge_color = color;
	return *this;
}

void BoxRenderer::Render()
{
	auto transform = gameObject()->transform();
	auto world = transform->GetMatrix();
	if (material.base_color >= 0)
		DrawBox(-Vector2::one / 2, Vector2::one / 2, material.base_color, true, world);
	if (material.edge_color >= 0)
		DrawBox(-Vector2::one / 2, Vector2::one / 2, material.edge_color, false, world);
}

void CircleRenderer::Render()
{
	auto transform = gameObject()->transform();
	auto world = transform->GetMatrix();
	if (material.base_color >= 0)
		DrawOval(Vector2::zero, Vector2::one, material.base_color, true, world);
	if (material.edge_color >= 0)
		DrawOval(Vector2::zero, Vector2::one, material.base_color, false, world);
}

void LineRenderer::Render()
{
	auto transform = gameObject()->transform();
	auto world = transform->GetMatrix();
	auto line = std::dynamic_pointer_cast<LineCollider>(gameObject()->GetComponent<Collider>())->GetShape(*gameObject()->transform());
	DrawLine(line.p1, line.p2, material.edge_color, world);
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
	auto& frame = texture.GetFrame();
	auto world = gameObject()->transform()->GetMatrix();
	auto graphworld = Matrix3::CreateScale(AspectUtils::Inner(frame.GetSize(), Vector2::one)) * world;
	frame.DrawGraph(graphworld);
}
