#ifndef SPRITE_ANIMATOR_H
#define SPRITE_ANIMATOR_H

#include "Core/ECS/Component.h"

namespace Engine
{
    class SpriteAnimator;

    class IState
    {
    public:
        virtual ~IState() {} // not an interface anymore but only way I saw to make the compiler stop complaining.
        virtual void OnStateEnter( SpriteAnimator* spriteAnimator) = 0;
        virtual void OnStateUpdate( SpriteAnimator* spriteAnimator) = 0;
        virtual void OnStateExit( SpriteAnimator* spriteAnimator) = 0;
    };
}

template<typename T>
concept StateType = std::is_base_of<Engine::IState, T>::value;

namespace Engine
{
	class SpriteAnimator final: public Component
	{
	public:
		SpriteAnimator(GameObject* object);

		virtual ~SpriteAnimator() = default;

		SpriteAnimator(const SpriteAnimator& other) = delete;
		SpriteAnimator(SpriteAnimator&& other) = delete;
		SpriteAnimator& operator=(const SpriteAnimator& other) = delete;
		SpriteAnimator& operator=(SpriteAnimator&& other) = delete;

		virtual void Update() override;
        virtual void Render() override;

        void SetState(const std::string& newState);

        template<StateType T>
        T* AddState(const std::string& stateName);

	private:
        std::unordered_map<std::string, std::unique_ptr<IState>> m_States{};
        IState* m_CurrentState{};
	};
}


template<StateType T>
T* Engine::SpriteAnimator::AddState(const std::string& stateName)
{
    if (!m_States.contains(stateName))
    {
        m_States[stateName] = std::make_unique<T>();
    }

    if (m_CurrentState == nullptr)
    {
        m_CurrentState = m_States[stateName].get();
    }

    L_TRACE("{} added to Animator for {}", stateName, GetOwner()->GetName());

    return static_cast<T*>(m_States[stateName].get());
}


#endif
