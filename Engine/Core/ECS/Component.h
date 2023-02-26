#ifndef COMPONENT_H
#define COMPONENT_H

namespace Engine
{
	// Forward declare
	class GameObject;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		// Optional life-cycle events
		virtual void ComponentInit() {}
		virtual void ComponentAttach() {}
		virtual void ComponentDetach() {}

		// Manditory life-cycle events.
		virtual void ComponentUpdate() = 0;
		virtual void ComponentRender() = 0;

	private:
		friend class GameObject;
		void SetOwner(GameObject* owner);

	protected:
		GameObject* GetOwner() const;

	private:
		GameObject* m_Owner{};
	};
}
#endif
