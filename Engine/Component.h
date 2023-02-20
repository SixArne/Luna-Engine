#ifndef COMPONENT_H
#define COMPONENT_H

namespace Engine
{
	static size_t REGISTERED_COMPONENT_COUNT = 0;

	template<typename T>
	size_t GetComponentId()
	{
		static const size_t id = REGISTERED_COMPONENT_COUNT++;
		return id;
	}

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		//Component(const Component& other) = delete;
		//Component(Component&& other) = delete;
		//Component& operator=(const Component& other) = delete;
		//Component& operator=(Component&& other) = delete;
	};
}
#endif // !Component
