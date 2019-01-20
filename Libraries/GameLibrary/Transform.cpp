#include "Transform.h"

Transform::Transform(const Vector2& position, float rotation, const Vector2& scale)
	: world(Matrix3::Identity)
	, position(position)
	, rotation(rotation)
	, scale(scale)
{
}

Transform::Transform()
	: Transform(Vector2{}, 0, Vector2{ 1, 1 })
{
}

Matrix3 Transform::GetLocalMatrix() const
{
	Matrix3 m = Matrix3::Identity;
	m *= Matrix3::CreateScale(scale);
	m *= Matrix3::CreateRotationZ(rotation);
	m *= Matrix3::CreateTranslation(position);
	m *= world;
	return m;
}

Matrix3 Transform::GetParentMatrix() const
{
	Matrix3 m = Matrix3::Identity;
	if (auto& p = parent.lock())
		m *= p->GetMatrix();
	return m;
}

Matrix3 Transform::GetMatrix() const
{
	return GetLocalMatrix() * GetParentMatrix();
}

/*
Transform Transform::Transformed() const
{
	Transform transform;
	if (auto ptransform = parent.lock())
		transform = ptransform->Transformed();
	transform.scale *= scale;
	transform.position += position.Rotate(transform.rotation);
	transform.rotation += rotation;
	return transform;
}
/**/