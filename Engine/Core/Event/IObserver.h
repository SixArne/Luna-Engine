#ifndef OBSERVER_H
#define OBSERVER_H

namespace Engine
{
	template<typename T>
	class IObserver
	{
	public:
		virtual void OnNotify(T data) = 0;
		virtual IObserver<T>* GetObserver() = 0;
	};
}

#endif // !OBSERVER_H
