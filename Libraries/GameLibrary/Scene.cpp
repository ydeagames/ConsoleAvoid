#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"

void Scene::Update()
{
	for (auto& object : objects)
		object.second->Start();

	for (auto& object : objects)
		object.second->Update();

	//objects.erase(std::remove_if(objects.begin(), objects.end(), [](std::pair<const std::string, std::shared_ptr<GameObject>>& obj)->bool { return obj.second->IsDestroyed(); }), objects.end());
	for (auto itr = objects.begin(); itr != objects.end();)
	{
		if (itr->second->IsDestroyed())
		{
			itr = objects.erase(itr);
			continue;
		}
		++itr;
	}
	for (auto& layer : layers)
		layer.erase(std::remove_if(layer.begin(), layer.end(), [](std::weak_ptr<GameObject>& obj)->bool { return obj.expired(); }), layer.end());
}

void Scene::Render()
{
	static auto& context = ScreenManager::GetInstance().GetOutputContext();

	if (first_rendering)
	{
		context.ClearStaticBuffer();
		first_rendering = false;
	}

	if (!context.HasStaticBuffer())
	{
		context.ClearStaticBuffer();
		context.BeginStaticBuffer();
		for (auto& layer : layers)
			for (auto& wobject : layer)
				if (auto object = wobject.lock())
					if (object && !object->IsDestroyed() && object->transform()->IsStaticObject())
						object->Render();
		context.EndStaticBuffer();
		context.Clear();
	}

	for (auto& layer : layers)
		for (auto& wobject : layer)
			if (auto object = wobject.lock())
				if (object && !object->IsDestroyed() && !object->transform()->IsStaticObject())
					object->Render();
}
