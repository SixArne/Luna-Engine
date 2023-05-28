#include "SpriteAnimator.h"
#include "GameObject.h"

Engine::SpriteAnimator::SpriteAnimator(GameObject* object)
	: Component{object}
{
}

void Engine::SpriteAnimator::Update()
{
    if (m_CurrentState != nullptr)
    {
        m_CurrentState->OnStateUpdate(this);
    }
}

void Engine::SpriteAnimator::Render()
{

}

void Engine::SpriteAnimator::SetState(const std::string& newState)
{
    if (m_CurrentState != nullptr)
    {
        m_CurrentState->OnStateExit(this);
    }

    if (m_States.contains(newState))
    {
        auto toEnterState = m_States.at(newState).get();
        toEnterState->OnStateEnter(this);
        m_CurrentState = toEnterState;
    }
}
