#ifndef LEVEL_INSTANCER_H
#define LEVEL_INSTANCER_H

#include <vector>
#include <memory>
#include <tuple>
#include "LevelLoader/Structs/Level.h"
#include "LevelLoader/Structs/GameSettings.h"
#include <GameObject.h>

namespace Galaga
{
    class LevelInstancer final
    {
    public:
        LevelInstancer(std::vector<Level>& levels, const GameSettings& gameSettings, std::tuple<int, int> windowSize);
        // will receive level and game settings and make the levels.

        // TODO: Player needs to be made and shared by all scenes.

    private:
        void CreateLevel(Level& level);

        using TexturesInfo = std::vector<std::shared_ptr<Engine::Texture2D>>;
        using EnemyTextures = std::tuple<TexturesInfo, TexturesInfo, TexturesInfo>;

        std::shared_ptr<Engine::GameObject> CreatePlayer();
        std::shared_ptr<Engine::GameObject> CreateHighScoreHud();
        std::shared_ptr<Engine::GameObject> CreateLivesHud();

        std::shared_ptr<Engine::GameObject> CreateBeeEnemy(EnemyTextures& textureInfo);
        // std::shared_ptr<Engine::GameObject> CreateButterflyEnemy(EnemyTextures& textureInfo);
        // std::shared_ptr<Engine::GameObject> CreateBossEnemy(EnemyTextures& textureInfo);

        int m_WindowWidth{};
        int m_WindowHeight{};
        GameSettings m_GameSettings{};
    };
}

#endif // LEVEL_INSTANCER