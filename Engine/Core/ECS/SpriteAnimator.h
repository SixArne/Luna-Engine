#ifndef SPRITE_ANIMATOR_H
#define SPRITE_ANIMATOR_H

#include "Core/ECS/Component.h"
#include "Texture2D.h"

namespace Engine
{
    class SpriteAnimator;

    class AbstractState
    {
    public:
        virtual ~AbstractState() {} // not an interface anymore but only way I saw to make the compiler stop complaining.
        virtual void OnStateEnter( SpriteAnimator* spriteAnimator) = 0;
        virtual void OnStateUpdate( SpriteAnimator* spriteAnimator) = 0;
        virtual void OnStateExit( SpriteAnimator* spriteAnimator) = 0;

        void SetTextures(std::vector<std::shared_ptr<Texture2D>>& textures) { m_Textures = textures; }
        std::vector<std::shared_ptr<Texture2D>>& GetTextures() { return m_Textures; }

        float GetSecondsPerFrame() const { return m_SecondsPerFrame; }
        int GetFrameToRender() const { return m_FrameToRender; }
        void SetFrameToRender(int frame) { m_FrameToRender = frame; }
        bool GetShouldLoopAnimation() const { return m_ShouldLoopAnimation; }

        int GetNextFrame() { return (++m_FrameToRender) % m_Textures.size(); }

    protected:
        std::vector<std::shared_ptr<Texture2D>> m_Textures{};

        int m_FrameToRender{};
        float m_SecondsPerFrame{0.3f};
        bool m_ShouldLoopAnimation{true};
    };
}

template<typename T>
concept StateType = std::is_base_of<Engine::AbstractState, T>::value;

namespace Engine
{
    class TextureRenderer;

	class SpriteAnimator final: public Component
	{
	public:
		SpriteAnimator(GameObject* object);

		virtual ~SpriteAnimator() = default;

		SpriteAnimator(const SpriteAnimator& other) = delete;
		SpriteAnimator(SpriteAnimator&& other) = delete;
		SpriteAnimator& operator=(const SpriteAnimator& other) = delete;
		SpriteAnimator& operator=(SpriteAnimator&& other) = delete;

        virtual void Init() override;
		virtual void Update() override;
        virtual void Render() override;

        void SetState(const std::string& newState);
        void Pause();
        void Resume();

        template<StateType T>
        T* AddState(const std::string& stateName);

	private:
        std::unordered_map<std::string, std::unique_ptr<AbstractState>> m_States{};
        AbstractState* m_CurrentState{};

        TextureRenderer* m_TextureRenderer{};

        bool m_IsPaused{};
        float m_StateTimer{};
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
