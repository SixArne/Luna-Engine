#ifndef FLY_BACK_STATE_H
#define FLY_BACK_STATE_H

#include "Core/ECS/SpriteAnimator.h"
#include "Texture2D.h"
#include "Util/BezierSpline.h"
#include "Components/Enemy/EnemyBug.h"

namespace Galaga
{
    class FlyBackState final : public Engine::AbstractState
    {
    public:
        FlyBackState() {};
        ~FlyBackState() {};

        virtual void OnStateEnter( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateUpdate( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateExit( Engine::SpriteAnimator* spriteAnimator) override;

    private:
        float m_Timer{};

        EnemyBug* m_EnemyBug{};

		Utils::BezierSpline m_Path;
        float m_AnimationTime{3.f};
    };
}

#endif //FLY_IN_STATE_H
