#ifndef XBOX_CONTROLLER_H
#define XBOX_CONTROLLER_H

#include <memory>

#include <glm/glm.hpp>

namespace Engine
{
	class XboxController final
	{
	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			ButtonA = 0x1000,
			ButtonB = 0x2000,
			ButtonX = 0x4000,
			ButtonY = 0x8000
		};

		enum class ControllerAxis
		{
			LeftThumbX = 0,
			LeftThumbY = 1,
			LeftThumb = 2,
			LeftTrigger = 4,
			RightTrigger = 5,
			RightThumbX = 6,
			RightThumbY = 7,
			RightThumb = 8
		};

		explicit XboxController(unsigned int controllerIndex);
		~XboxController();

		void Update();
		bool IsPressedThisFrame(ControllerButton button) const;  // flank detection
		bool IsReleasedThisFrame(ControllerButton button) const;  // flank detection
		bool IsPressed(ControllerButton button) const;

		float GetLeftTrigger() const;
		float GetRightTrigger() const;

		glm::vec2 GetLeftThumbStick() const;
		glm::vec2 GetRightThumbStick() const;
	
	private:
		class XboxControllerImpl;
		std::unique_ptr<XboxControllerImpl> m_ControllerImpl;
	};
}

#endif