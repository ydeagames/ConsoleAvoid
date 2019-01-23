#pragma once
#include "Event.h"

class CollisionEvent : public Event
{
public:
	std::shared_ptr<GameObject> other;
	CollisionResult result;

public:
	CollisionEvent(const std::shared_ptr<GameObject>& other, const CollisionResult& result)
		: other(other)
		, result(result) {}
};

class CollisionEnterEvent : public CollisionEvent
{
public:
	CollisionEnterEvent(const std::shared_ptr<GameObject>& other, const CollisionResult& result)
		: CollisionEvent(other, result) {}
};

class TriggerEnterEvent : public CollisionEvent
{
public:
	TriggerEnterEvent(const std::shared_ptr<GameObject>& other, const CollisionResult& result)
		: CollisionEvent(other, result) {}
};