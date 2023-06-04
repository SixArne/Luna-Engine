#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <functional>
#include <glm/glm.hpp>

#include "Core/ECS/Component.h"
#include "GameObject.h"

namespace Engine
{
    struct RigidBody2DCollider
    {
        float width;
        float height;
    };

    class RigidBody2D final: public Component
    {
    public:
		RigidBody2D(GameObject* object) : Component(object) {};
        RigidBody2D(GameObject* object, const RigidBody2DCollider collider) :  Component(object), m_Collider{collider} {}
        ~RigidBody2D();

    public:
        void Init() override;
        void Update() override {};
        void Render() override {};
        void OnSceneLoad() override;
        void OnSceneUnload() override;

        RigidBody2DCollider GetCollider() const { return m_Collider	; }
        uint64_t GetEntityID() const { return m_EntityID; }

        glm::vec2 GetPosition() const { return GetOwner()->GetTransform()->GetWorldPosition(); };

        void SetOnCollisionEnterCallback(std::function<void(RigidBody2D*)> callback) { m_OnCollisionEnterCallback = callback; }
        void SetOnCollisionExitCallback(std::function<void(RigidBody2D*)> callback) { m_OnCollisionExitCallback = callback; }
        void SetOnCollisionCallback(std::function<void(RigidBody2D*)> callback) { m_OnCollisionCallback = callback; }

        void OnCollisionEnter(RigidBody2D* other) { if (m_OnCollisionEnterCallback) m_OnCollisionEnterCallback(other); }
        void OnCollisionExit(RigidBody2D* other) { if (m_OnCollisionExitCallback) m_OnCollisionExitCallback(other); }
        void OnCollision(RigidBody2D* other) { if (m_OnCollisionCallback) m_OnCollisionCallback(other); }

    private:
        void Remove();

        std::function<void(RigidBody2D*)> m_OnCollisionEnterCallback{};
        std::function<void(RigidBody2D*)> m_OnCollisionExitCallback{};
        std::function<void(RigidBody2D*)> m_OnCollisionCallback{};

        RigidBody2DCollider m_Collider{};
        uint64_t m_EntityID{};

        uint64_t static s_EntityID;
    };
}

#endif // !RIGIDBODY_H