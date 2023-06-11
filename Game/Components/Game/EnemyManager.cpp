#include "EnemyManager.h"
#include <GameObject.h>
#include <Scene.h>
#include <Core/Event/EventManager.h>
#include <SceneManager.h>

void Galaga::EnemyManager::Init()
{
    auto result = GetOwner()->GetScene()->GetObjectsByTag("enemy");
    m_EnemiesCount = static_cast<int>(result.size());

    Engine::EventManager::GetInstance().AttachEvent("EnemyDead", [this](Engine::Event*){
        OnEnemyDeath();
    });
}

void Galaga::EnemyManager::Update()
{
    [[maybe_unused]]int i = 0;
}

void Galaga::EnemyManager::Render()
{
}

void Galaga::EnemyManager::OnEnemyDeath()
{
    m_EnemiesCount--;

    if (m_EnemiesCount == 0)
    {
	    Engine::EventManager::GetInstance().Notify("EndGame", nullptr);
        Engine::SceneManager::GetInstance().GetNextScene();
    }
}