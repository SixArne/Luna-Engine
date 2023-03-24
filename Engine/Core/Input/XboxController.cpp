#include "pch.h"

#include <Xinput.h>
#include "XboxController.h"
#include "InputManager.h"

class Engine::XboxController::XboxControllerImpl
{
public:
	void Update()
	{
		CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
		XInputGetState(0, &m_CurrentState);

		auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
		m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
		m_ButtonsPressedLastFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
	}

	bool IsDownThisFrame(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	};

	bool IsUpThisFrame(unsigned int button) const
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
		auto leftXnormalized = m_CurrentState.Gamepad.sThumbLX / 255.f;
		auto leftYnormalized = m_CurrentState.Gamepad.sThumbLY / 255.f;

		glm::vec2 displacement{ leftXnormalized, -leftYnormalized };

		if (IsLeftInDeadZone())
		{
			return glm::vec2(0);
		}

		return glm::normalize(displacement);
	};

	glm::vec2 GetRightThumbStick() const
	{
		auto rightXnormalized = m_CurrentState.Gamepad.sThumbRX / 255.f;
		auto rightYnormalized = m_CurrentState.Gamepad.sThumbRY / 255.f;

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

	using ControllerCommandsMap = std::map<int, std::pair<InputState, std::unique_ptr<Command>>>;
	ControllerCommandsMap m_ConsoleCommands{};
};

Engine::XboxController::XboxController()
{
}

Engine::XboxController::XboxController(int)
	:pImpl{std::make_unique<XboxControllerImpl>()}
{
}

Engine::XboxController::~XboxController()
{
}

void Engine::XboxController::Update()
{
	pImpl->Update();
}

bool Engine::XboxController::IsDownThisFrame(unsigned int button) const
{
	return pImpl->IsDownThisFrame(button);
}

bool Engine::XboxController::IsUpThisFrame(unsigned int button) const
{
	return pImpl->IsUpThisFrame(button);
}

bool Engine::XboxController::IsPressed(unsigned int button) const
{
	return pImpl->IsPressed(button);
}

float Engine::XboxController::GetLeftTrigger() const
{
	return pImpl->GetLeftTrigger();
}

float Engine::XboxController::GetRightTrigger() const
{
	return pImpl->GetRightTrigger();
}

glm::vec2 Engine::XboxController::GetLeftThumbStick() const
{
	return pImpl->GetLeftThumbStick();
}

glm::vec2 Engine::XboxController::GetRightThumbStick() const
{
	return pImpl->GetRightThumbStick();
}
