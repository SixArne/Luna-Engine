#include "FlyInState.h"
#include <Core/Time.h>

void Galaga::FlyInState::OnStateEnter( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("FlyInState::OnStateEnter");
}

void Galaga::FlyInState::OnStateUpdate( Engine::SpriteAnimator* spriteAnimator)
{
    L_DEBUG("FlyInState::OnStateUpdate");

    m_Timer += Engine::Time::GetInstance().GetDeltaTime();
    if (m_Timer >= m_Duration)
    {
        spriteAnimator->SetState("attack");
    }
}

void Galaga::FlyInState::OnStateExit( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("FlyInState::OnStateExit");
    m_Timer = 0.0f;
}