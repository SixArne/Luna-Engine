#include "pch.h"

#include "Scene.h"
#include "GameObject.h"

using namespace Engine;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Init()
{
	for (auto& object : m_objects)
	{
		object->Init();
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		if (object->IsActive())
		{
			object->Update();
		}
	}
}

void Engine::Scene::FixedUpdate(float fdt)
{
	for (auto& object : m_objects)
	{
		if (object->IsActive())
		{
			object->FixedUpdate(fdt);
		}
	}
}

void Engine::Scene::LateUpdate()
{
	std::vector<std::shared_ptr<GameObject>> objectsToDestroy{};

	for (const auto& object : m_objects)
	{
		// NEVER DELETE IN A LOOP
		if (object->IsMarkedForDeletion())
		{
			L_TRACE("[{}] Removed object: {}", m_name.c_str(), object->GetName());
			objectsToDestroy.push_back(object);

			continue;
		}

		if (object->IsActive())
		{
			object->LateUpdate();
		}
	}

	for (const auto& object : objectsToDestroy)
	{
		Remove(object);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		if (object->IsActive())
		{
			object->Render();
		}
	}
}

void Engine::Scene::OnImGui()
{
	for (const auto& object : m_objects)
	{
		if (object->IsActive())
		{
			object->OnImGui();
		}
	}
}

void Scene::Instantiate(std::shared_ptr<GameObject> object)
{
	Add(object);
	object->Init();
}