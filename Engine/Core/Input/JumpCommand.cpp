#include "pch.h"
#include "JumpCommand.h"
#include "GameObject.h"

void Engine::JumpCommand::Execute()
{
	L_DEBUG("Jump command executed by {}", m_GameObject->GetName());
}
