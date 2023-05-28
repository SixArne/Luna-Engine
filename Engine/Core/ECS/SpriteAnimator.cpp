#include "SpriteAnimator.h"
#include "GameObject.h"
#include "TextureRenderer.h"
#include "Core/Time.h"
#include "Core/Log.h"

Engine::SpriteAnimator::SpriteAnimator(GameObject* object)
	: Component{object}
{
}

void Engine::SpriteAnimator::Init()
{
    m_TextureRenderer = GetOwner()->GetComponent<TextureRendererComponent>();
    if (m_TextureRenderer == nullptr)
    {
        L_ERROR("SpriteAnimator requires a TextureRendererComponent");
    }
}

void Engine::SpriteAnimator::Pause()
{
    m_IsPaused = true;
}

void Engine::SpriteAnimator::Resume()
{
    m_IsPaused = false;
}

void Engine::SpriteAnimator::Update()
{
    if (m_CurrentState == nullptr) return;
    if (m_IsPaused) return;

    m_CurrentState->OnStateUpdate(this);
    m_StateTimer += TIME.GetDeltaTime();

    if (m_StateTimer >= m_CurrentState->GetSecondsPerFrame())
    {
        if (m_CurrentState->GetTextures().size() > 0)
        {
            int frameToRender = m_CurrentState->GetNextFrame();
            m_TextureRenderer->SetTexture(m_CurrentState->GetTextures()[frameToRender]);
            m_StateTimer = 0.f;

            if (frameToRender == m_CurrentState->GetTextures().size() - 1 && !m_CurrentState->GetShouldLoopAnimation())
            {
                m_TextureRenderer->SetEmptyTexture();
                m_CurrentState->OnStateExit(this);
                m_CurrentState = nullptr;
            }
        }
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
        m_StateTimer = 0.f;
    }
}
