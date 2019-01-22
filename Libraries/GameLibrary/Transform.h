#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
	std::weak_ptr<Transform> parent;// <�e>
	Matrix3 world;					// <�s��>
	Vector2 position;				// <�e�N�X�`��>
	float rotation;					// <��]>
	Vector2 scale;					// <�X�P�[��>
	bool static_object;				// <�ÓI�I�u�W�F�N�g>

public:
	Transform(const Vector2& position, float rotation, const Vector2& scale);
	Transform();

public:
	Matrix3 GetLocalMatrix() const;
	Matrix3 GetParentMatrix() const;
	Matrix3 GetMatrix() const;
	bool IsStaticObject() const;
};
