#include "AttackState.h"
#include <Core/Log.h>
#include <Core/Time.h>
#include "Util/Random.h"

void Galaga::AttackState::OnStateEnter( Engine::SpriteAnimator* spriteAnimator)
{
    L_DEBUG("AttackState::OnStateEnter");

    m_EnemyBug = spriteAnimator->GetOwner()->GetComponent<Galaga::EnemyBug>();
    const float randomX = Utils::Random::GetInstance().GetRandomFloat(-600, 600.f);
    const float randomY = Utils::Random::GetInstance().GetRandomFloat(0, 600.f);
    
    auto endPosition = glm::vec2
    {
        randomX,
        600.f
    };

   
    auto startPosition = spriteAnimator->GetOwner()->GetTransform()->GetLocalPosition();
    m_StartPosition = startPosition;

    std::vector<glm::vec2> positions{
        startPosition,
        glm::vec2{212.12074f, randomY},
        glm::vec2{randomX, 412.770f },
        endPosition // this x is end position same with y
    };

    m_Path = Utils::BezierSpline(positions);
}

void Galaga::AttackState::OnStateUpdate( Engine::SpriteAnimator* spriteAnimator)
{
    // L_DEBUG("AttackState::OnStateUpdate");
    m_Timer += Engine::Time::GetInstance().GetDeltaTime();

    m_Path.Update(m_Timer / m_AnimationTime);
    spriteAnimator->GetOwner()->GetTransform()->SetLocalPosition(m_Path.GetTravellersPosition());

    if (m_Timer >= m_AnimationTime)
    {
        spriteAnimator->SetState("flyBack");
    }
}

void Galaga::AttackState::OnStateExit( Engine::SpriteAnimator* /*spriteAnimator*/)
{
    L_DEBUG("AttackState::OnStateExit");

    m_EnemyBug->SetTargetPosition(m_StartPosition);

    m_Timer = 0.0f;
}