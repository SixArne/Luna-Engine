
#include "SoundVolumeCommand.h"
#include <Core/Services/ServiceLocator.h>
#include <Core/Services/Sound/ISoundService.h>

namespace Galaga
{
    SoundVolumeCommand::SoundVolumeCommand(GameObject* gobj, Type type)
        : Command{ gobj }, m_Type{ type }
    {
    }

    void SoundVolumeCommand::Execute(InputData)
    {
        auto ss = Engine::ServiceLocator::GetSoundService();
        switch (m_Type)
        {
        case Type::Increase:
            ss->IncreaseVolume();
            break;
        case Type::Decrease:
            ss->DecreaseVolume();
            break;
        case Type::Mute:
            ss->Mute();
            break;
        default:
            break;
        }
    }
}

