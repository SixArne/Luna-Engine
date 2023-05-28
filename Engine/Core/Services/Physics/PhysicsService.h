#ifndef PHYSICS_SERVICE_H
#define PHYSICS_SERVICE_H

#include <vector>
#include <set>
#include <unordered_map>

#include "IPhysicsService.h"


namespace Engine
{
    class RigidBody2D;

    class PhysicsService final: public IPhysicsService
    {
    public:
        PhysicsService() {}
        ~PhysicsService() {}
        void AddRigidBody(RigidBody2D* rb);
        void RemoveRigidBody(RigidBody2D* rb);

        void Update();

    private:
        std::vector<RigidBody2D*> m_RigidBodies;

        std::unordered_map<uint64_t, std::set<uint64_t>> m_Collisions{};
    };
}

#endif // PHYSICS_SERVICE_H