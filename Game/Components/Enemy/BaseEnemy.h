#ifndef GALAGA_BASE_ENEMY_H
#define GALAGA_BASE_ENEMY_H

#include <Core/ECS/Component.h>

#include <GameObject.h>

namespace Galaga
{
	using Engine::Component;
    using Engine::GameObject;

    class BaseEnemy : public Component
    {
    public:
        BaseEnemy(GameObject* object);
        virtual ~BaseEnemy() = default;

        BaseEnemy(const BaseEnemy& other) = delete;
        BaseEnemy(BaseEnemy&& other) = delete;
        BaseEnemy& operator=(const BaseEnemy& other) = delete;
        BaseEnemy& operator=(BaseEnemy&& other) = delete;

        virtual void Init() override;
        virtual void Update() override;
        virtual void Render() override;

        void SetHasFinishedIntroFlight(bool hasFinishedIntroFlight) { m_HasFinishedIntroFlight = hasFinishedIntroFlight; }
        bool HasFinishedIntroFlight() const { return m_HasFinishedIntroFlight; }

    private:
        bool m_HasFinishedIntroFlight{false};
    };
}

#endif //GALAGA_BASE_ENEMY_H