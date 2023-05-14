#ifndef SDL_SOUND_SYSTEM_H
#define SDL_SOUND_SYSTEM_H

#include "ISoundService.h"
#include <string>
#include <memory>

namespace Engine
{
    class SDLSoundSystem final : public ISoundService
    {
    public:
        SDLSoundSystem(int channelCount = 8);
        ~SDLSoundSystem();

        void Play(const std::string& soundName, const float volume) override;

    private:
        class SDLSoundSystemImpl; // Forward declaration of the implementation class
        std::unique_ptr<SDLSoundSystemImpl> m_impl; // Pointer to the implementation

        void Stop();
    };
}

#endif // SDL_SOUND_SYSTEM_H