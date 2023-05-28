#include "AttackState.h"
#include <Core/Log.h>
#include <Core/Time.h>

void Galaga::AttackState::OnStateEnter( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("AttackState::OnStateEnter");
}

void Galaga::AttackState::OnStateUpdate( Engine::SpriteAnimator*)
{
    // L_DEBUG("AttackState::OnStateUpdate");
}

void Galaga::AttackState::OnStateExit( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("AttackState::OnStateExit");
    m_Timer = 0.0f;
}