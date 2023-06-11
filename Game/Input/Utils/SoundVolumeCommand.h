#ifndef UTILS_SOUND_VOLUME_COMMAND_H
#define UTILS_SOUND_VOLUME_COMMAND_H

#include <Core/Input/Command.h>

using Engine::Command;
using Engine::GameObject;
using Engine::InputData;

namespace Galaga
{
    class SoundVolumeCommand final: public Command
    {
    public:
        enum class Type
        {
            Increase,
            Decrease,
            Mute
        };

        SoundVolumeCommand(GameObject* gobj, Type type);
        virtual ~SoundVolumeCommand() = default;
        virtual void Execute(InputData data) override;

    private:
        Type m_Type{};
    };
}

#endif // !UTILS_SOUND_VOLUME_COMMAND_H