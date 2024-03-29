#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Event/Subject.h"
#include "Core/Event/IObserver.h"

namespace Engine
{
	// Forward declare
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		// Optional life-cycle events
		virtual void Init() {}
		virtual void Attach() {}
		virtual void Detach() {}
		virtual void FixedUpdate(float) {}
		virtual void LateUpdate() {}
		virtual void OnImGui() {};
		virtual void OnSceneLoad() {};
		virtual void OnSceneUnload() {};

		// Manditory life-cycle events.
		virtual void Update() = 0;
		virtual void Render() = 0;
		inline bool IsMarkedForDeletion() { return m_MarkedForDeletion; }
		bool GetCanBeRemoved();
		GameObject* GetOwner() const;

		friend class GameObject;

	private:
		inline void MarkForDeletion() { m_MarkedForDeletion = true; }

	protected:
		explicit Component(GameObject* pOwner) : m_pOwner{ pOwner } {}
		void SetCanBeRemoved(bool value);

	private:
		GameObject* m_pOwner{};
		bool m_MarkedForDeletion{};
		bool m_CanBeRemoved{};
	};
}
#endif
