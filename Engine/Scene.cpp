#include "Scene.h"
#include "GameObject.h"
#include <string>
#include <future>

#include "Core/Log.h"

using namespace Engine;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object, bool isPersistant)
{
	if (!isPersistant)
	{
		object->SetScene(this);
		m_objects.emplace_back(std::move(object));
	}
	else
	{
		object->SetScene(this);
		m_PersistantObjects.emplace_back(std::move(object));
	}
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemovePersistant(std::shared_ptr<GameObject> object)
{
	m_PersistantObjects.erase(std::remove(m_PersistantObjects.begin(), m_PersistantObjects.end(), object), m_PersistantObjects.end());
}

void Scene::Init()
{
	for (auto& object : m_objects)
	{
		object->Init();
	}

	for (auto& object: m_PersistantObjects)
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

	for(auto& object : m_PersistantObjects)
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

	for (auto& object : m_PersistantObjects)
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

	for (const auto& object : m_PersistantObjects)
	{
		// NEVER DELETE IN A LOOP
		if (object->IsMarkedForDeletion() && object->CanBeDestroyed())
		{
			L_TRACE("[{}] Removed object: {}", m_name.c_str(), object->GetName());
			m_PersistantObjectsToDestroy.push_back(object);

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

	for (const auto& object : m_PersistantObjectsToDestroy)
	{
		RemovePersistant(object);
	}

	m_PersistantObjectsToDestroy.clear();
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

	for (const auto& object : m_PersistantObjects)
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

	for (const auto& object : m_PersistantObjects)
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

	for (const auto& object : m_PersistantObjects)
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

	for (const auto& object : m_PersistantObjects)
	{
		object->OnSceneUnload();
	}
}

void Engine::Scene::Instantiate(std::shared_ptr<GameObject> object, bool isPersistant)
{
	if (!isPersistant)
	{
		Add(object);
	}
	else
	{
		Add(object, true);
	}

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

	for (const auto& object : m_PersistantObjects)
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

void Engine::Scene::ClearPersistantObjects()
{
	for (auto& object: m_PersistantObjects)
	{
		object->MarkForDeletion();
	}

	auto cleanedUp = std::async(std::launch::async, [&] {
		while (true)
		{
			bool isCleanedUp = true;

			for (auto& object: m_PersistantObjects)
			{
				if (!object->CanBeDestroyed())
				{
					isCleanedUp = false;
					break;
				}
			}

			// As long as something isn't cleaned up, keep waiting
			if (!isCleanedUp)
			{
				continue;
			}
			else
			{
				break;
			}
		}
	});
	cleanedUp.wait();

	m_PersistantObjects.clear();
}

void Scene::RemoveAll()
{
	for (auto& object: m_objects)
	{
		object->MarkForDeletion();
	}

	auto cleanedUp = std::async(std::launch::async, [&] {
		while (true)
		{
			bool isCleanedUp = true;

			for (auto& object: m_objects)
			{
				if (!object->CanBeDestroyed())
				{
					isCleanedUp = false;
					break;
				}
			}

			// As long as something isn't cleaned up, keep waiting
			if (!isCleanedUp)
			{
				continue;
			}
			else
			{
				break;
			}
		}
	});
	cleanedUp.wait();

	m_objects.clear();
}


const std::vector<std::shared_ptr<GameObject>> Engine::Scene::GetPersistantObjects()
{
	return m_PersistantObjects;
}

void Engine::Scene::Reset()
{
	ClearPersistantObjects();
	RemoveAll();

	m_IsInitialized = false;
}