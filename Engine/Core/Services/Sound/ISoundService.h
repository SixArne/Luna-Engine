#ifndef ISOUNDSERVICE_H
#define ISOUNDSERVICE_H

#include <string>

namespace Engine
{
    class ISoundService
    {
    public:
        virtual ~ISoundService() = default;
        virtual void Play(const std::string& soundName, const float volume = 0.5f) = 0;
        virtual void IncreaseVolume() = 0;
        virtual void DecreaseVolume() = 0;
        virtual void Mute() = 0;
    };

    class NullSoundService final : public ISoundService
    {
    public:
        NullSoundService() = default;
        ~NullSoundService() = default;

        void Play(const std::string& /*soundName*/, const float /*volume*/) override {};
        void IncreaseVolume() override {};
        void DecreaseVolume() override {};
        void Mute() override {};
    };
}

#endif // ISOUNDSERVICE_H