#ifndef IPHYSICS_SERVICE_H
#define IPHYSICS_SERVICE_H


namespace Engine
{
    class RigidBody2D;

    class IPhysicsService
    {
    public:
        virtual ~IPhysicsService() = default;

        virtual void AddRigidBody(RigidBody2D* rb) = 0;
        virtual void Update() = 0;
        virtual void CleanAll() = 0;

    };

    class NullPhysicsService final : public IPhysicsService
    {
    public:
        NullPhysicsService() = default;
        ~NullPhysicsService() = default;

        virtual void AddRigidBody(RigidBody2D*) {};
        virtual void Update() {};
        virtual void CleanAll() {};
    };
}

#endif // ISOUNDSERVICE_H