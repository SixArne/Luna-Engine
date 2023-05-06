#include "pch.h"
#include "Projectile.h"

#include <GameObject.h>
#include <Core/Time.h>
#include <glm/vec2.hpp>

Galaga::Projectile::Projectile(GameObject* object, glm::vec2 direction, glm::vec2 speed)
    : Component{ object }, m_Direction{ direction }, m_Speed{ speed }
{
}

void Galaga::Projectile::Init()
{
}

void Galaga::Projectile::Update()
{
    GetOwner()->GetTransform()->AddLocalPosition({ m_Speed * Engine::TIME.GetDeltaTime()});
}

void Galaga::Projectile::Render()
{
}
