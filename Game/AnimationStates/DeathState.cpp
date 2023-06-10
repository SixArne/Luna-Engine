#include "DeathState.h"
#include <Core/Time.h>
#include <Core/Log.h>

void Galaga::DeathState::OnStateEnter( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("DeathState::OnStateEnter");

    m_ShouldLoopAnimation = false;
}

void Galaga::DeathState::OnStateUpdate( Engine::SpriteAnimator*)
{
    // L_DEBUG("FlyInState::OnStateUpdate");


}

void Galaga::DeathState::OnStateExit( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("DeathState::OnStateExit");
    m_Timer = 0.0f;
}