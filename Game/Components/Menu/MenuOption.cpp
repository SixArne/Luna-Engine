#include "MenuOption.h"

#include <ResourceManager.h>

#include <Core/ECS/TextComponent.h>
#include <Core/ECS/TextureRenderer.h>

#include <GameObject.h>

Galaga::MenuOption::MenuOption(GameObject* object)
    : Component(object)
{
}

Galaga::MenuOption::MenuOption(GameObject* object, const std::string& text, const std::function<void()>& onClick, bool isSelected)
    : Component(object)
    , m_OnClick(onClick)
    , m_SelectedTexture(Engine::ResourceManager::GetInstance().LoadTexture("Resources/Sprites/Menu/MenuOptionSelected.png"))
    , m_IsSelected(isSelected)
    , m_Text(text)
{
}

void Galaga::MenuOption::Init()
{
    m_TextComponent = GetOwner()->AddComponent<TextComponent>();
    m_TextComponent->SetText(m_Text);
    m_TextComponent->SetColor(glm::ivec3(255, 255, 255));
    auto size = m_TextComponent->GetScreenSize();
    m_TextComponent->SetOffset(glm::ivec2{size.x / 2.f, 10});

    m_TextureRenderer = GetOwner()->AddComponent<TextureRenderer>();

    if (m_IsSelected)
    {
        m_TextureRenderer->SetTexture(m_SelectedTexture);
    }
    else
    {
        m_TextureRenderer->SetEmptyTexture();
    }
}

void Galaga::MenuOption::Click()
{
    if (m_IsSelected)
    {
        m_OnClick();
    }
}

void Galaga::MenuOption::Select()
{
    m_TextureRenderer->SetTexture(m_SelectedTexture);
    m_IsSelected = true;
}

void Galaga::MenuOption::Deselect()
{
    m_TextureRenderer->SetEmptyTexture();
    m_IsSelected = false;
}