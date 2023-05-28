#include "AttackState.h"
#include <Core/Log.h>
#include <Core/Time.h>

void Galaga::AttackState::OnStateEnter( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("AttackState::OnStateEnter");
}

void Galaga::AttackState::OnStateUpdate( Engine::SpriteAnimator* spriteAnimator)
{
    L_DEBUG("AttackState::OnStateUpdate");

    m_Timer += Engine::Time::GetInstance().GetDeltaTime();
    if (m_Timer >= m_Duration)
    {
        spriteAnimator->SetState("fly_in");
    }
}

void Galaga::AttackState::OnStateExit( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("AttackState::OnStateExit");
    m_Timer = 0.0f;
}