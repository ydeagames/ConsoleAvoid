#pragma once
#include "Component.h"

class CollisionEvent : public Component
{
public:
	CollisionEvent() = default;
	virtual ~CollisionEvent() = default;

public:
	virtual void OnCollisionEnter(GameObject& other) {}
	virtual void OnTriggerEnter(GameObject& other) {}
};