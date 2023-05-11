#include "pch.h"
#include "SDLSoundSystem.h"

#include <SDL_mixer.h>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <filesystem>
#include <ResourceManager.h>

using namespace std::literals::chrono_literals;

Engine::SDLSoundSystem::SDLSoundSystem(int channelCount)
    : m_initialized{false},
    // Call like lambda because we need to pass in the stop token
    m_soundThread{ [this](std::stop_token stopToken) { SoundThread(stopToken); } },
    m_channelCount{ channelCount }
{
}

Engine::SDLSoundSystem::~SDLSoundSystem()
{
    m_soundThread.request_stop();
}

void Engine::SDLSoundSystem::Play(const std::string& soundName, const float volume)
{
    {
        // Lock the queue, this will be unlocked when the lock goes out of scope
        std::unique_lock<std::mutex> lock(m_queueMutex);

        // Add new song to queue.
        m_SoundQueue.push({ soundName, volume });
    }

    // Set the condition variable to notify the thread that there is a new song to play
    m_queueCondition.notify_one();
}

void Engine::SDLSoundSystem::SoundThread(std::stop_token stopToken)
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
    int audioChannels = Mix_AllocateChannels(m_channelCount);
    for (int i = 0; i < audioChannels; i++)
    {
        m_OpenChannels.push_back(i);
    }

    L_TRACE("Allocated {0} audio channels", audioChannels);

    // Keep looping until the stop token is requested
    // This is generated automatically by the jthread
    while (!stopToken.stop_requested())
    {
        SoundRequest request{};
        {
            // Lock the queue, this will be unlocked when the lock goes out of scope
            std::unique_lock<std::mutex> lock(m_queueMutex);

            // Wait until there is a new song to play
            // Then check if the queue has a song in it or if the stop token has been requested
            m_queueCondition.wait(lock, [&]() {
                return !m_SoundQueue.empty() || stopToken.stop_requested();
            });

            // If the stop token has been requested, break out of the loop
            if (stopToken.stop_requested())
            {
                break;
            }

            // Get the next song to play
            request = m_SoundQueue.front();

            // Remove the song from the queue
            m_SoundQueue.pop();
        }

        // Play sound here
        // TODO: Fix delay, memory leaks and multi sound play
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
            int volume = static_cast<int>(MIX_MAX_VOLUME * request.volume);
            Mix_VolumeMusic(volume);

            if (m_OpenChannels.size() > 0)
            {
                int openChannel = m_OpenChannels.front();

                if (Mix_PlayChannel(openChannel, sound, 0) == -1)
                {
                    L_ERROR("Failed to play sound: {0}! SDL_mixer Error: {1}", request.soundName, Mix_GetError());
                }

                m_playedSounds.push_back({sound, openChannel});
                m_OpenChannels.erase(m_OpenChannels.begin());
            }
            else
            {
                L_WARN("No open channels to play sound: {0}", request.soundName);
            }
        }

        // Go over playedSounds and remove the ones that are done playing
        std::vector<Mix_Chunk*> finishedSounds{};
        for (auto channel: m_playedSounds)
        {
            if (Mix_Playing(channel.second) == 0)
            {
                finishedSounds.push_back(channel.first);
            }
        }

        for (Mix_Chunk* finishedSound: finishedSounds)
        {
            // find where sound is sound.first and free audio and remove from list
            Mix_FreeChunk(finishedSound);

            auto it = std::find_if(m_playedSounds.begin(), m_playedSounds.end(), [finishedSound](const auto& playedSound) {
                return playedSound.first == finishedSound;
            });

            if (it != m_playedSounds.end())
            {
                m_OpenChannels.push_back(it->second);
                m_playedSounds.erase(it);
            }
        }
    }

    Mix_CloseAudio();
}