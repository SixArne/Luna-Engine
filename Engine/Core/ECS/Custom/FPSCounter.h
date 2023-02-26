#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "Core/ECS/Component.h"

namespace Engine
{
	class FPSCounter final: public Component
	{
	public:
		FPSCounter() = default;
		virtual ~FPSCounter() = default;

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;
		FPSCounter& operator=(const FPSCounter& other) = delete;
		FPSCounter& operator=(FPSCounter&& other) = delete;

		virtual void ComponentInit() override;
		virtual void ComponentUpdate() override;
		virtual void ComponentRender() override;
	};
}

#endif
