#ifndef GALAGA_GAME_ENEMY_MANAGER_H
#define GALAGA_GAME_ENEMY_MANAGER_H

#include <Core/ECS/Component.h>

#include <functional>
#include <memory>

namespace Galaga
{
    using Engine::GameObject;
    using Engine::Component;

    class EnemyManager final: public Component
    {
    public:
        EnemyManager(GameObject* object) : Component(object) {}
        virtual ~EnemyManager() override = default;

		EnemyManager(const EnemyManager& other) = delete;
		EnemyManager(EnemyManager&& other) = delete;
		EnemyManager& operator=(const EnemyManager& other) = delete;
		EnemyManager& operator=(EnemyManager&& other) = delete;

        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() override;

    private:
        void OnEnemyDeath();

        int m_EnemiesCount{0};
    };
}

#endif // GALAGA_GAME_ENEMY_MANAGER_H_