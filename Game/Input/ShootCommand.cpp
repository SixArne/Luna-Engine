#include "ShootCommand.h"

#include <GameObject.h>
#include <Core/Log.h>
#include "Components/Player/SpaceFighter.h"

Galaga::ShootCommand::ShootCommand(GameObject* gobj)
    : Command{ gobj }
{
    m_SpaceFighter = gobj->GetComponent<SpaceFighter>();

    if (m_SpaceFighter == nullptr)
    {
        L_ERROR("SpaceFighter component not found but expected on [{}]", gobj->GetName());
    }
}

void Galaga::ShootCommand::Execute(InputData)
{
    m_SpaceFighter->Shoot();
	L_DEBUG("Shoot");
}
