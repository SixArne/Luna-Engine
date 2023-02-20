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
