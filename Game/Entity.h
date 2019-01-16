#pragma once

class Entity
{
public:
	Vector2 pos;

public:
	Entity() = default;
	virtual ~Entity() = default;

public:
	virtual void Update() = 0;
	virtual void Render() = 0;
};

