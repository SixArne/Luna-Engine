#ifndef ATTACK_STATE_H
#define ATTACK_STATE_H

#include "Core/ECS/SpriteAnimator.h"
#include "Texture2D.h"
#include "Util/BezierSpline.h"
#include "Components/Enemy/EnemyBug.h"

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
        float m_AnimationTime{ 2.0f };
		Utils::BezierSpline m_Path{};

        EnemyBug* m_EnemyBug{};

        glm::vec2 m_StartPosition{};
    };
}

#endif //FLY_IN_STATE_H
