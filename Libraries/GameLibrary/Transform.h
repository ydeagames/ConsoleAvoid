#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
	std::weak_ptr<Transform> parent;// <親>
	Matrix3 world;					// <行列>
	Vector2 position;				// <テクスチャ>
	float rotation;					// <回転>
	Vector2 scale;					// <スケール>

public:
	Transform(const Vector2& position, float rotation, const Vector2& scale);
	Transform();

public:
	Matrix3 GetLocalMatrix() const;
	Matrix3 GetParentMatrix() const;
	Matrix3 GetMatrix() const;
};
