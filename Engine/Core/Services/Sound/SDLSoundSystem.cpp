#include "pch.h"

#include "SDLSoundSystem.h"
#include <SDL_mixer.h>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <filesystem>
#include <atomic>
#include <ResourceManager.h>

namespace Engine
{
    class SDLSoundSystem::SDLSoundSystemImpl
    {
    public:
        SDLSoundSystemImpl(int channelCount);
        ~SDLSoundSystemImpl();

        void Play(const std::string& soundName, const float volume);
        void IncreaseVolume();
        void DecreaseVolume();
        void Mute();
        void Stop();

    private:
        void SoundThread();

        struct SoundRequest
        {
            std::string soundName;
            float volume;
        };

        std::queue<SoundRequest> m_SoundQueue{};
        std::mutex m_queueMutex{};
        std::condition_variable m_queueCondition{};
        std::atomic<bool> m_initialized{ false };
        std::thread m_soundThread;

        // music to channels.
        using QueuedSong = std::pair<Mix_Chunk*, int>;
        std::vector<QueuedSong> m_playedSounds{};

        // Open channels
        std::vector<int> m_OpenChannels{};

        // Channel count
        int m_channelCount;

        std::atomic<bool> m_stopRequested{ false };

        // Volume
        float m_Volume{0.5f};
        float m_LastVolume{0.5f};

        bool m_IsMuted{ false };
    };
}

//////////////////////////////////////////
// SDLSoundSystem Implementation
//////////////////////////////////////////

Engine::SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl(int channelCount)
    : m_initialized(false),
    m_soundThread([this]() { SoundThread(); }),
    m_channelCount(channelCount) {}

Engine::SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
    Stop();
}

void Engine::SDLSoundSystem::SDLSoundSystemImpl::Play(const std::string& soundName, const float volume)
{
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);

        if (volume <= FLT_EPSILON)
        {
            m_SoundQueue.push({ soundName, m_Volume });
        }
        else
        {
            m_SoundQueue.push({ soundName, volume });
        }
    }
    m_queueCondition.notify_one();
}

void Engine::SDLSoundSystem::SDLSoundSystemImpl::Stop()
{
    m_stopRequested = true;
    m_queueCondition.notify_one();
    m_soundThread.join();
}

void Engine::SDLSoundSystem::SDLSoundSystemImpl::IncreaseVolume()
{
    m_Volume = std::clamp(m_Volume + 0.05f, 0.f, 1.f);
}

void Engine::SDLSoundSystem::SDLSoundSystemImpl::DecreaseVolume()
{
    m_Volume = std::clamp(m_Volume - 0.05f, 0.f, 1.f);
}

void Engine::SDLSoundSystem::SDLSoundSystemImpl::Mute()
{
    if (!m_IsMuted)
    {
        m_LastVolume = m_Volume;
        m_Volume = 0.f;
    }
    else
    {
        m_Volume = m_LastVolume;
    }

    m_IsMuted = !m_IsMuted;
}

void Engine::SDLSoundSystem::SDLSoundSystemImpl::SoundThread()
{
    m_initialized.store(true);

    // Initialize SDL_mixer
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        L_ERROR("SDL_mixer could not initialize! SDL_mixer Error: {0}", Mix_GetError());
        m_initialized.store(false);
        return;
    }

    // Created channels and add them as open.
    m_channelCount = Mix_AllocateChannels(m_channelCount);
    for (int i = 0; i < m_channelCount; i++)
    {
        m_OpenChannels.push_back(i);
    }

    L_TRACE("Allocated {0} audio channels", m_channelCount);

    // Keep looping until the stop token is requested
    // This is generated automatically by the jthread
    while (true)
    {
        SoundRequest request{};
        {
            // Lock the queue, this will be unlocked when the lock goes out of scope
            std::unique_lock<std::mutex> lock(m_queueMutex);

            // Wait until there is a new song to play
            // Then check if the queue has a song in it or if the stop token has been requested
            m_queueCondition.wait(lock, [&]() {
                return !m_SoundQueue.empty() || m_stopRequested;
            });

            // If the stop token has been requested, break out of the loop
            if (m_stopRequested)
            {
                for (auto song: m_playedSounds)
                {
                    Mix_FreeChunk(song.first);
                }

                break;
            }

            // Get the next song to play
            request = m_SoundQueue.front();

            // Remove the song from the queue
            m_SoundQueue.pop();
        }

        // Go over playedSounds and remove the ones that are done playing
        std::vector<Mix_Chunk*> finishedSounds{};
        for (auto channel: m_playedSounds)
        {
            if (Mix_Playing(channel.second) == 0)
            {
                finishedSounds.push_back(channel.first);
                m_OpenChannels.push_back(channel.second);
                m_playedSounds.erase(std::remove(m_playedSounds.begin(), m_playedSounds.end(), channel), m_playedSounds.end());
            }
        }

        // Reverse for loop over finished sounds and delete the last element after calling Mix_FreeChunk
        for (size_t i{}; i < finishedSounds.size(); i++)
        {
            Mix_FreeChunk(finishedSounds[i]);
            finishedSounds.pop_back();
        }

        // Play sound here
        auto source = ResourceManager::GetInstance().LoadSound(request.soundName);
        Mix_Chunk* sound = Mix_LoadWAV(source.c_str());
        if (sound == NULL)
        {
            auto cwd = std::filesystem::current_path();
            L_DEBUG("Current path: {0}", cwd.string());
            L_ERROR("Failed to load sound: {0}! SDL_mixer Error: {1}", source, Mix_GetError());
        }
        else
        {
            int volume = static_cast<int>(MIX_MAX_VOLUME * m_Volume);
            Mix_VolumeChunk(sound, volume);

            // Take the back channel and remove from list
            if (m_OpenChannels.empty())
            {
                L_ERROR("No open channels available for: {0}! SDL_mixer Error: {1}", request.soundName, Mix_GetError());
                continue;
            }

            int channel = m_OpenChannels.back();
            m_OpenChannels.pop_back();

            int openChannel = Mix_PlayChannel(channel, sound, 0);
            if (openChannel == -1)
            {
                L_ERROR("Failed to find channel for: {0}! SDL_mixer Error: {1}", request.soundName, Mix_GetError());
            }
            m_playedSounds.push_back({sound, channel});
        }
    }

    L_TRACE("Sound thread stopped");
    Mix_CloseAudio();
}


//////////////////////////////////////////
// SDLSoundSystem
//////////////////////////////////////////

Engine::SDLSoundSystem::SDLSoundSystem(int channelCount)
    : m_impl(std::make_unique<SDLSoundSystemImpl>(channelCount))
{}

Engine::SDLSoundSystem::~SDLSoundSystem() = default;

void Engine::SDLSoundSystem::Play(const std::string& soundName, const float volume)
{
    m_impl->Play(soundName, volume);
}

void Engine::SDLSoundSystem::Stop()
{
    m_impl->Stop();
}

void Engine::SDLSoundSystem::IncreaseVolume()
{
    m_impl->IncreaseVolume();
}

void Engine::SDLSoundSystem::DecreaseVolume()
{
    m_impl->DecreaseVolume();
}

void Engine::SDLSoundSystem::Mute()
{
    m_impl->Mute();
}


