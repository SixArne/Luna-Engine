#ifndef LOGGING_SOUND_SYSTEM_H
#define LOGGING_SOUND_SYSTEM_H

#include "ISoundService.h"

namespace Engine
{
    class LoggingSoundSystem final : public ISoundService
    {
    public:
        LoggingSoundSystem(ISoundService* soundService);
        ~LoggingSoundSystem();

        void Play(const std::string& soundName, const float volume) override;
        void IncreaseVolume() override;
        void DecreaseVolume() override;
        void Mute() override;

    private:
        ISoundService* m_soundService;
    };
}

#endif // LOGGING_SOUND_SYSTEM_H