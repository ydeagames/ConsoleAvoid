#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "Collider.h"
#include "CollisionEvent.h"

void Rigidbody::AddForce(const Vector2& velocity)
{
	vel += velocity;
}

void Rigidbody::Update()
{
	auto& object1 = gameObject();
	for (int hit_layer : hit_layers)
	{
		auto& layer = GameObject::FindGameObjectsWithLayer(hit_layer);
		for (auto& wobject : layer)
			if (auto object2 = wobject.lock())
				if (object2 && !object2->IsDestroyed() && object1 != object2)
				{
					auto collider1 = object1->GetComponent<Collider>();
					auto collider2 = object2->GetComponent<Collider>();
					if (collider1 && collider2)
					{
						CollisionResult result = collider1->Collide(*collider2);
						if (result.hit)
						{
							{
								if (!collider1->isTrigger && !collider2->isTrigger)
									collider1->Apply(result);
								auto& eventbus1 = object1->eventbus();
								if (collider1->isTrigger || collider2->isTrigger)
									eventbus1->Post(TriggerEnterEvent{ object2, result });
								else
									eventbus1->Post(CollisionEnterEvent{ object2, result });
							}
							//if (!object2->HasComponent<Rigidbody>())
							{
								if (!collider1->isTrigger && !collider2->isTrigger)
									collider2->Apply(result);
								auto& eventbus2 = object2->eventbus();
								if (collider1->isTrigger || collider2->isTrigger)
									eventbus2->Post(TriggerEnterEvent{ object1, result });
								else
									eventbus2->Post(CollisionEnterEvent{ object1, result });
							}
						}
					}
				}
	}

	gameObject()->transform()->position += vel * Time::deltaTime;
}