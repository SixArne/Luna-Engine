#include "MoveCommand.h"

#include <Core/Input/Command.h>
#include <GameObject.h>
#include <Core/Time.h>
#include <glm/glm.hpp>
#include <variant>
#include <Core/Log.h>

using Engine::Time;

void Galaga::MoveCommand::Execute(InputData data)
{
    auto currentPosition = m_GameObject->GetTransform()->GetWorldPosition();

    // No movement when border is reached

    if (std::holds_alternative<glm::vec2>(data))
    {
        glm::vec2 offset = std::get<glm::vec2>(data);
        glm::vec2 delta = glm::vec2{offset.x, 0.f};

        if (currentPosition.x + offset.x <= 80)
            return;
        else if (currentPosition.x + offset.x >= 380)
            return;

        m_GameObject->GetTransform()->AddLocalPosition(delta * m_PixelsPerSecond * TIME.GetDeltaTime());
    }
    else
    {
        if (currentPosition.x + m_Movement.x <= 80)
            return;
        else if (currentPosition.x + m_Movement.x >= 380)
            return;

        m_GameObject->GetTransform()->AddLocalPosition(m_Movement * m_PixelsPerSecond * TIME.GetDeltaTime());
    }
}