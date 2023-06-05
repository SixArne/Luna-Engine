#ifndef LEVEL_INSTANCER_H
#define LEVEL_INSTANCER_H

#include <vector>
#include <memory>
#include <tuple>
#include "LevelLoader/Structs/Level.h"
#include "LevelLoader/Structs/GameSettings.h"
#include <GameObject.h>
#include <Scene.h>

namespace Galaga
{
    class LevelInstancer final : public Engine::Singleton<LevelInstancer>
    {
    public:
        // will receive level and game settings and make the levels.
        void Load(std::vector<Level>& levels, const GameSettings& gameSettings, std::tuple<int, int> windowSize);

    private:
        friend class Engine::Singleton<LevelInstancer>;
        LevelInstancer() = default;

        Engine::Scene& CreateLevel(Level& level);
        Engine::Scene& CreateMenu();

        void OnSinglePlayer();
        void OnMultiPlayer();
        void OnVersus();

        using TexturesInfo = std::vector<std::shared_ptr<Engine::Texture2D>>;
        using EnemyTextures = std::tuple<TexturesInfo, TexturesInfo, TexturesInfo>;

        std::shared_ptr<Engine::GameObject> CreatePlayer();
        std::shared_ptr<Engine::GameObject> CreateHighScoreHud();
        std::shared_ptr<Engine::GameObject> CreateLivesHud();
        std::shared_ptr<Engine::GameObject> CreateLevelName(const std::string& levelName);

        std::shared_ptr<Engine::GameObject> CreateBeeEnemy(EnemyTextures& textureInfo);
        // std::shared_ptr<Engine::GameObject> CreateButterflyEnemy(EnemyTextures& textureInfo);
        // std::shared_ptr<Engine::GameObject> CreateBossEnemy(EnemyTextures& textureInfo);

        int m_WindowWidth{};
        int m_WindowHeight{};
        GameSettings m_GameSettings{};

        std::shared_ptr<Engine::GameObject> m_Player{};
    };
}

#endif // LEVEL_INSTANCER