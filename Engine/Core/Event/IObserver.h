#ifndef OBSERVER_H
#define OBSERVER_H

namespace Engine
{
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void OnNotify(const std::string&) {};
	};
}

#endif // !OBSERVER_H
