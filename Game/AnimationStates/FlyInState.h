#ifndef FLY_IN_STATE_H
#define FLY_IN_STATE_H

#include "Core/ECS/SpriteAnimator.h"
#include "Texture2D.h"
#include "Util/BezierSpline.h"

namespace Galaga
{
    class FlyInState final : public Engine::AbstractState
    {
    public:
        FlyInState() {};
        ~FlyInState() {};

        virtual void OnStateEnter( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateUpdate( Engine::SpriteAnimator* spriteAnimator) override;
        virtual void OnStateExit( Engine::SpriteAnimator* spriteAnimator) override;

    private:
        float m_Timer{};

		Utils::BezierSpline m_Path;
        float m_AnimationTime{3.f};
    };
}

#endif //FLY_IN_STATE_H
