#ifndef COMPONENT_H
#define COMPONENT_H

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
		virtual void ComponentInit() {}
		virtual void ComponentAttach() {}
		virtual void ComponentDetach() {}
		virtual void ComponentFixedUpdate(float) {}
		virtual void ComponentLateUpdate() {}

		// Manditory life-cycle events.
		virtual void ComponentUpdate() = 0;
		virtual void ComponentRender() = 0;

	private:
		friend class GameObject;
		//friend void SetOwner(GameObject* owner);

		inline void MarkForDeletion() { m_MarkedForDeletion = true; }
		inline bool IsMarkedForDeletion() { return m_MarkedForDeletion; }

	protected:
		explicit Component(GameObject* pOwner) : m_pOwner{ pOwner } {}
		GameObject* GetOwner() const;

	private:
		GameObject* m_pOwner{};
		bool m_MarkedForDeletion{};
	};
}
#endif
