#ifndef PHYSICS_SERVICE_H
#define PHYSICS_SERVICE_H

#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
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
        void CleanAll();

        void Update();

    private:
        void PhysicsLoop();
        void Stop();

        std::vector<RigidBody2D*> m_RigidBodies;
        std::vector<RigidBody2D*> m_NewBodies{};
        std::unordered_set<RigidBody2D*> m_BodiesToErase{};

        std::mutex m_PhysicsMutex{};
        std::mutex m_AddNewBodiesMutex{};
        std::condition_variable m_PhysicsCondition{};
        std::condition_variable m_AddNewBodiesCondition{};
        std::atomic<bool> m_stopRequested{ false };
        std::jthread m_PhysicsThread;


        // rb id to list of colliding rb ids
        std::unordered_map<RigidBody2D*, std::set<RigidBody2D*>> m_Collisions{};
    };
}

#endif // PHYSICS_SERVICE_H