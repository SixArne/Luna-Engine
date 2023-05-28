#ifndef PHYSICS_SERVICE_H
#define PHYSICS_SERVICE_H

#include <vector>
#include <set>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <thread>

#include "IPhysicsService.h"


namespace Engine
{
    class RigidBody2D;

    class PhysicsService final: public IPhysicsService
    {
    public:
        PhysicsService();
        ~PhysicsService();
        void AddRigidBody(RigidBody2D* rb);
        void RemoveRigidBody(RigidBody2D* rb);

        void Update();

    private:
        void PhysicsLoop();
        void Stop();

        std::vector<RigidBody2D*> m_RigidBodies;

        std::mutex m_PhysicsMutex{};
        std::condition_variable m_PhysicsCondition{};
        std::atomic<bool> m_stopRequested{ false };
        std::jthread m_PhysicsThread;


        // rb id to list of colliding rb ids
        std::unordered_map<uint64_t, std::set<uint64_t>> m_Collisions{};
    };
}

#endif // PHYSICS_SERVICE_H