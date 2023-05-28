#ifndef DEATH_STATE_H
#define DEATH_STATE_H

#include "Core/ECS/SpriteAnimator.h"
#include "Texture2D.h"

namespace Galaga
{
    class DeathState final : public Engine::AbstractState
    {
    public:
        DeathState() {};
        ~DeathState() {};

        virtual void OnStateEnter( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateUpdate( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateExit( Engine::SpriteAnimator* spriteAnimator) override;

    private:
        float m_Timer{};
        float m_Duration{ 10.0f };
    };
}

#endif //DEATH_STATE_H
