#include "FlyBackState.h"

#include <Core/Time.h>
#include "GameObject.h"
#include "Components/Enemy/EnemyBug.h"
#include "Util/Random.h"
#include <Core/Log.h>

void Galaga::FlyBackState::OnStateEnter( Engine::SpriteAnimator* spriteAnimator)
{
    L_DEBUG("FlyInState::OnStateEnter");

    m_EnemyBug = spriteAnimator->GetOwner()->GetComponent<Galaga::EnemyBug>();
    auto endPosition = m_EnemyBug->GetTargetPosition();

    if (m_EnemyBug != nullptr)
    {
        m_EnemyBug->SetHasFinishedIntroFlight(false);
    }

    std::vector<glm::vec2> positions{
        glm::vec2{-100, -100},
        glm::vec2{200.f, -200.f},
        glm::vec2{200.f, -200.f},
        endPosition // this x is end position same with y
    };

    m_Path = Utils::BezierSpline(positions);
}

void Galaga::FlyBackState::OnStateUpdate( Engine::SpriteAnimator* spriteAnimator)
{
    // L_DEBUG("FlyInState::OnStateUpdate");

    m_Timer += Engine::Time::GetInstance().GetDeltaTime();

    m_Path.Update(m_Timer / m_AnimationTime);
    spriteAnimator->GetOwner()->GetTransform()->SetLocalPosition(m_Path.GetTravellersPosition());

    if (m_Timer >= m_AnimationTime)
    {
        spriteAnimator->SetState("idle");
    }
}

void Galaga::FlyBackState::OnStateExit( Engine::SpriteAnimator*)
{
    L_DEBUG("FlyBackState::OnStateExit");

    if (m_EnemyBug != nullptr)
    {
        m_EnemyBug->SetHasFinishedIntroFlight(true);
    }

    m_Timer = 0.0f;
}