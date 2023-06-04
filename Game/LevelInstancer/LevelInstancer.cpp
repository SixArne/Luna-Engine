#include "LevelInstancer.h"
#include <SceneManager.h>
#include <Scene.h>

Galaga::LevelInstancer::LevelInstancer(std::vector<Level>& levels, const GameSettings& gameSettings)
    :m_GameSettings{gameSettings}
{
    for (auto& level: levels)
    {
        CreateLevel(level);
    }
}

void Galaga::LevelInstancer::CreateLevel(Level& level)
{
    Engine::Scene& scene = Engine::SceneManager::GetInstance().CreateScene(level.name);

    for (int i{}; i < level.boss.count; ++i)
    {

    }
}
