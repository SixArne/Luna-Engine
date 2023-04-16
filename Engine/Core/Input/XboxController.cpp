#include "pch.h"

#include <Xinput.h>
#include "XboxController.h"
#include "InputManager.h"

#define THUMB_MAX 32767.0f

class Engine::XboxController::XboxControllerImpl
{
public:
	XboxControllerImpl(unsigned int controllerIndex)
		: m_ControllerIndex{ controllerIndex }
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
	}

	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(0, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsPressedLastFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsPressedThisFrame(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	};

	bool IsReleasedThisFrame(unsigned int button) const
	{
		return m_ButtonsPressedLastFrame & button;
	};
	bool IsPressed(unsigned int button) const
	{
		return m_CurrentState.Gamepad.wButtons & button;
	};

	float GetLeftTrigger() const
	{
		return m_CurrentState.Gamepad.bLeftTrigger / 255.f;
	};

	float GetRightTrigger() const
	{
		return m_CurrentState.Gamepad.bRightTrigger / 255.f;
	};

	bool IsLeftInDeadZone() const
	{
		short sX = m_CurrentState.Gamepad.sThumbLX;
		short sY = m_CurrentState.Gamepad.sThumbLY;

		if (sX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || sX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return false;
		if (sY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || sY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			return false;

		return true;
	};

	bool IsRightInDeadZone() const
	{
		short sX = m_CurrentState.Gamepad.sThumbRX;
		short sY = m_CurrentState.Gamepad.sThumbRY;

		if (sX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || sX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return false;

		if (sY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || sY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
			return false;

		return true;
	};

	glm::vec2 GetLeftThumbStick() const
	{
		auto leftXnormalized = m_CurrentState.Gamepad.sThumbLX / THUMB_MAX;
		auto leftYnormalized = m_CurrentState.Gamepad.sThumbLY / THUMB_MAX;

		glm::vec2 displacement{ leftXnormalized, -leftYnormalized };

		if (IsLeftInDeadZone())
		{
			return glm::vec2(0);
		}

		return glm::normalize(displacement);
	};

	glm::vec2 GetRightThumbStick() const
	{
		auto rightXnormalized = m_CurrentState.Gamepad.sThumbRX / THUMB_MAX;
		auto rightYnormalized = m_CurrentState.Gamepad.sThumbRY / THUMB_MAX;

		glm::vec2 displacement{ rightXnormalized, rightYnormalized };

		if (IsRightInDeadZone())
		{
			return glm::vec2(0);
		}

		return glm::normalize(displacement);
	};

private:
	XINPUT_STATE m_CurrentState{};
	XINPUT_STATE m_PreviousState{};

	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsPressedLastFrame{};

	unsigned int m_ControllerIndex{};
};

Engine::XboxController::XboxController(unsigned int controllerIndex)
	:m_ControllerImpl{std::make_unique<XboxControllerImpl>(controllerIndex)}
{
}

Engine::XboxController::~XboxController()
{
}

void Engine::XboxController::Update()
{
	m_ControllerImpl->Update();
}

bool Engine::XboxController::IsPressedThisFrame(ControllerButton button) const
{
	return m_ControllerImpl->IsPressedThisFrame(static_cast<unsigned int>(button));
}

bool Engine::XboxController::IsReleasedThisFrame(ControllerButton button) const
{
	return m_ControllerImpl->IsReleasedThisFrame(static_cast<unsigned int>(button));
}

bool Engine::XboxController::IsPressed(ControllerButton button) const
{
	return m_ControllerImpl->IsPressed(static_cast<unsigned int>(button));
}

float Engine::XboxController::GetLeftTrigger() const
{
	return m_ControllerImpl->GetLeftTrigger();
}

float Engine::XboxController::GetRightTrigger() const
{
	return m_ControllerImpl->GetRightTrigger();
}

glm::vec2 Engine::XboxController::GetLeftThumbStick() const
{
	return m_ControllerImpl->GetLeftThumbStick();
}

glm::vec2 Engine::XboxController::GetRightThumbStick() const
{
	return m_ControllerImpl->GetRightThumbStick();
}
