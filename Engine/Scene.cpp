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

	m_IsInitialized = true;
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
	for (const auto& object : m_objects)
	{
		// NEVER DELETE IN A LOOP
		if (object->IsMarkedForDeletion() && object->CanBeDestroyed())
		{
			L_TRACE("[{}] Removed object: {}", m_name.c_str(), object->GetName());
			m_objectsToDestroy.push_back(object);

			continue;
		}

		if (object->IsActive())
		{
			object->LateUpdate();
		}
	}

	for (const auto& object : m_objectsToDestroy)
	{
		Remove(object);
	}

	m_objectsToDestroy.clear();
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

void Engine::Scene::OnLoad()
{
	for (const auto& object : m_objects)
	{
		object->OnSceneLoad();
	}
}

void Engine::Scene::OnUnload()
{
	for (const auto& object : m_objects)
	{
		object->OnSceneUnload();
	}
}

void Engine::Scene::Instantiate(std::shared_ptr<GameObject> object)
{
	Add(object);
	object->Init();
}

std::shared_ptr<GameObject> Engine::Scene::FindByName(const std::string& name) const
{
	for (const auto& object : m_objects)
	{
		if (object->GetName() == name)
		{
			return object;
		}
	}

	return nullptr;
}

bool Engine::Scene::IsInitialized()
{
	return m_IsInitialized;
}