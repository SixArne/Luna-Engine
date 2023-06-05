#include "pch.h"

#include "NavigateMenuCommand.h"
#include <GameObject.h>
#include <Core/ECS/Component.h>
#include "Components/Menu/OptionContainer.h"
#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/ISoundService.h>

Galaga::NavigateMenuCommand::NavigateMenuCommand(GameObject* gobj, NavigateMenuCommandType type) : Command{ gobj }, m_Type{ type }
{
    m_OptionContainer = m_GameObject->GetComponent<OptionContainer>();
}

void Galaga::NavigateMenuCommand::Execute(InputData)
{
    switch(m_Type)
    {
    case NavigateMenuCommandType::Next:
        m_OptionContainer->Next();
        Engine::ServiceLocator::GetSoundService()->Play("Resources/Audio/Menu/MenuNavigate.wav", 0.5f);
        break;

    case NavigateMenuCommandType::Previous:
        m_OptionContainer->Previous();
        Engine::ServiceLocator::GetSoundService()->Play("Resources/Audio/Menu/MenuNavigate.wav", 0.5f);
        break;

    case NavigateMenuCommandType::Click:
        Engine::ServiceLocator::GetSoundService()->Play("Resources/Audio/Menu/MenuNavigate.wav", 0.5f);
        m_OptionContainer->Click();
        break;

    default:
        break;
    }

}