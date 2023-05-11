#ifndef SDL_SOUND_SYSTEM_H
#define SDL_SOUND_SYSTEM_H

#include "ISoundService.h"
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

#include <SDL_mixer.h>

namespace Engine
{
    struct SoundRequest
    {
        std::string soundName;
        float volume;
    };

    class SDLSoundSystem final : public ISoundService
    {
    public:
        SDLSoundSystem(int channelCount = 8);
        ~SDLSoundSystem();

        void Play(const std::string& soundName, const float volume) override;

    private:
        void SoundThread(std::stop_token stopToken);

        std::queue<SoundRequest> m_SoundQueue{};
        std::mutex m_queueMutex{};
        std::condition_variable m_queueCondition{};
        std::atomic<bool> m_initialized{ false };
        std::jthread m_soundThread;

        // music to channels.
        using QueuedSong = std::pair<Mix_Chunk*, int>;
        std::vector<QueuedSong> m_playedSounds{};

        // open channels to music.
        std::vector<int> m_OpenChannels{};

        // Channel count
        const int m_channelCount;
    };
}

#endif // SDL_SOUND_SYSTEM_H