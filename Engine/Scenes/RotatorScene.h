#ifndef ROTATOR_SCENE_H
#define ROTATOR_SCENE_H

#include "Scene.h"

namespace Engine
{
	class RotatorScene final : public Scene
	{
	public:
		friend class TransformComponent;
		explicit RotatorScene(const std::string& scenename);
		virtual void OnImGui() override;
	};
}

#endif
