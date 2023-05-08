#include "pch.h"
#include "LivesIndicator.h"

#include <GameObject.h>
#include <Core/Event/EventManager.h>
#include <Core/ECS/TextComponent.h>
#include <Core/ECS/TextureRenderer.h>

#include <imgui.h>
#include <Core/Log.h>

#include <iostream>

Galaga::LivesIndicator::LivesIndicator(GameObject* gameobject, std::vector<TextureRendererComponent*>& liveRenderers)
	:Component{ gameobject }, m_LivesIcons{ liveRenderers }, m_Lives{ static_cast<int>(liveRenderers.size()) }
{
}

void Galaga::LivesIndicator::Init()
{
    m_CurrentLives = m_Lives;
}

void Galaga::LivesIndicator::Update()
{
}

void Galaga::LivesIndicator::Render()
{
}

void Galaga::LivesIndicator::OnNotify(int /*data*/)
{
    --m_CurrentLives;

	UpdateLives();
}

void Galaga::LivesIndicator::UpdateLives()
{
    for (int i{}; i < m_Lives; i++)
    {
        if (i < m_CurrentLives)
        {
            m_LivesIcons[i]->GetOwner()->SetActive(true);
        }
        else
        {
            m_LivesIcons[i]->GetOwner()->SetActive(false);
        }
    }
}

void Galaga::LivesIndicator::OnImGui()
{
    if (ImGui::CollapsingHeader("Lives Indicator"))
    {
        ImGui::InputInt("Lives", &m_CurrentLives);
    }

    UpdateLives();
}