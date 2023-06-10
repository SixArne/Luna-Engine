#include "SwitchSceneCommand.h"

#include <GameObject.h>
#include <Core/Input/Command.h>
#include <SceneManager.h>
#include <Core/Log.h>

void Galaga::SwitchSceneCommand::Execute(InputData)
{
    L_DEBUG("Switching scenes");
    Engine::SceneManager::GetInstance().GetNextScene();
}