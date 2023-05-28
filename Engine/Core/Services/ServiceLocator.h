#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "Sound/ISoundService.h"
#include "Physics/IPhysicsService.h"

namespace Engine
{
    class ISoundService;
    class IPhysicsService;

    class ServiceLocator final
    {
    public:
        ServiceLocator(const ServiceLocator& other) = delete;
        ServiceLocator(ServiceLocator&& other) = delete;
        ServiceLocator& operator=(const ServiceLocator& other) = delete;
        ServiceLocator& operator=(ServiceLocator&& other) = delete;

        static void RegisterSoundService(ISoundService* soundService);
        static void RegisterPhysicsService(IPhysicsService* physicsService);
        static ISoundService* GetSoundService();
        static IPhysicsService* GetPhysicsService();

        static void DestroyServices();

    private:
        ServiceLocator() = default;
        ~ServiceLocator() = default;

        inline static NullSoundService m_defaultSoundService{};
        inline static NullPhysicsService m_defaultPhysicsService{};

        static ISoundService* m_soundService;
        static IPhysicsService* m_physicsService;
    };
}

#endif // SERVICELOCATOR_H