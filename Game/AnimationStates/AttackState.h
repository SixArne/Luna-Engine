#ifndef ATTACK_STATE_H
#define ATTACK_STATE_H

#include "Core/ECS/SpriteAnimator.h"
#include "Texture2D.h"

namespace Galaga
{
    class AttackState final : public Engine::AbstractState
    {
    public:
        AttackState() {};
        ~AttackState() {};

        virtual void OnStateEnter( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateUpdate( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateExit( Engine::SpriteAnimator* spriteAnimator) override;

    private:
        float m_Timer{};
        float m_Duration{ 2.0f };
    };
}

#endif //FLY_IN_STATE_H
