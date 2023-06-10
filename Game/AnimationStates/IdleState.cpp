#include "IdleState.h"
#include <Core/Time.h>
#include <Core/Log.h>
#include "Util/Random.h"

void Galaga::IdleState::OnStateEnter( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("IdleState::OnStateEnter");

    m_Duration = Utils::Random::GetInstance().GetRandomFloat(5.f, 10.f);
}

void Galaga::IdleState::OnStateUpdate( Engine::SpriteAnimator* spriteAnimator)
{
    // L_DEBUG("FlyInState::OnStateUpdate");

    m_Timer += Engine::Time::GetInstance().GetDeltaTime();
    if (m_Timer >= m_Duration)
    {
        spriteAnimator->SetState("attack");
    }
}

void Galaga::IdleState::OnStateExit( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("IdleState::OnStateExit");
    m_Timer = 0.0f;
}