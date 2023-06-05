#include "OptionContainer.h"
#include <GameObject.h>
#include <Core/ECS/Component.h>
#include <algorithm>

Galaga::OptionContainer::OptionContainer(GameObject* object)
    : Component(object)
{
}

Galaga::OptionContainer::OptionContainer(GameObject* object, Options& options)
    : Component(object), m_Options{options}
{
}

void Galaga::OptionContainer::Next()
{
    if (m_Options.empty())
    {
        return;
    }

    m_Options[m_CurrentIndex]->Deselect();

    // This will bound check it for us
    m_CurrentIndex = std::clamp(m_CurrentIndex + 1, 0, static_cast<int>(m_Options.size() - 1));

    m_Options[m_CurrentIndex]->Select();
}

void Galaga::OptionContainer::Previous()
{
    if (m_Options.empty())
    {
        return;
    }

    m_Options[m_CurrentIndex]->Deselect();

    // This will bound check it for us
    m_CurrentIndex = std::clamp(m_CurrentIndex - 1, 0, static_cast<int>(m_Options.size() - 1));

    m_Options[m_CurrentIndex]->Select();
}

void Galaga::OptionContainer::AddOption(MenuOption* option)
{
    m_Options.push_back(option);
}

void Galaga::OptionContainer::Click()
{
    if (m_Options.empty())
    {
        return;
    }

    m_Options[m_CurrentIndex]->Click();
}