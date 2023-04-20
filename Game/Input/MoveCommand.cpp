#include "pch.h"
#include "MoveCommand.h"

#include <Core/Input/Command.h>
#include <GameObject.h>
#include <Core/Time.h>
#include <glm/glm.hpp>
#include <variant>

using Engine::Time;

void Galaga::MoveCommand::Execute(InputData data)
{
   if (std::holds_alternative<glm::vec2>(data))
    {
        glm::vec2 offset = std::get<glm::vec2>(data);
        m_GameObject->GetTransform()->AddLocalPosition(offset * TIME.GetDeltaTime());
    }
    else
    {
        m_GameObject->GetTransform()->AddLocalPosition(m_Movement * TIME.GetDeltaTime());
    }
}