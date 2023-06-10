#include "AutoKill.h"

#include <GameObject.h>
#include <Core/Time.h>

Galaga::AutoKill::AutoKill(GameObject* object, float lifetime)
    : Component{ object }, m_Lifetime{ lifetime }
{
}

void Galaga::AutoKill::Init()
{
}

void Galaga::AutoKill::Update()
{
    m_TimeAlive += Engine::TIME.GetDeltaTime();
    if (m_TimeAlive >= m_Lifetime)
    {
        GetOwner()->Destroy();
    }
}

void Galaga::AutoKill::Render()
{
}

void Galaga::AutoKill::Kill()
{
    GetOwner()->Destroy();
}