#include "FlyInState.h"
#include <Core/Time.h>
#include "GameObject.h"
#include "Components/Enemy/EnemyBug.h"
#include "Util/Random.h"
#include <Core/Log.h>

void Galaga::FlyInState::OnStateEnter( Engine::SpriteAnimator* spriteAnimator)
{
    L_DEBUG("FlyInState::OnStateEnter");

    auto bugComponent = spriteAnimator->GetOwner()->GetComponent<Galaga::EnemyBug>();
    auto endPosition = bugComponent->GetTargetPosition();

    const float randomX = Utils::Random::GetInstance().GetRandomFloat(-600, 600.f);
    const float randomY = Utils::Random::GetInstance().GetRandomFloat(0, 600.f);
    auto startPosition = glm::vec2{ randomX, 800.0f };

    std::vector<glm::vec2> positions{
        startPosition,
        glm::vec2{212.12074f, randomY},
        glm::vec2{randomX, 412.770f },
        endPosition // this x is end position same with y
    };

    m_Path = Utils::BezierSpline(positions);
}

void Galaga::FlyInState::OnStateUpdate( Engine::SpriteAnimator* spriteAnimator)
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

void Galaga::FlyInState::OnStateExit( Engine::SpriteAnimator* spriteAnimator)
{
    L_DEBUG("FlyInState::OnStateExit");
    spriteAnimator->GetOwner()->GetComponent<Galaga::EnemyBug>()->SetHasFinishedIntroFlight(true);

    m_Timer = 0.0f;
}