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

Engine::SDLSoundSystem::SDLSoundSystem()
    : m_initialized{false}, 
    // Call like lambda because we need to pass in the stop token
    m_soundThread{ [this](std::stop_token stopToken) { SoundThread(stopToken); } }
{   
}

Engine::SDLSoundSystem::~SDLSoundSystem()
{
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        L_ERROR("SDL_mixer could not initialize! SDL_mixer Error: {0}", Mix_GetError());
        m_initialized.store(false);
        return;
    }

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

        auto source = ResourceManager::GetInstance().LoadSound(request.soundName);
        Mix_Music* sound = Mix_LoadMUS(source.c_str());
        if (sound == nullptr)
        {
            auto cwd = std::filesystem::current_path();
	        L_DEBUG("Current path: {0}", cwd.string());
            L_ERROR("Failed to load sound: {0}! SDL_mixer Error: {1}", source, Mix_GetError());
        }
        else
        {
            Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * request.volume));
            if (Mix_PlayMusic(sound, 0) == -1)
            {
                L_ERROR("Failed to play sound: {0}! SDL_mixer Error: {1}", request.soundName, Mix_GetError());
            }
            else
            {
                if (stopToken.stop_requested())
                {
                    Mix_HaltMusic();
                    break;
                }
            }

            Mix_FreeMusic(sound);
        }
    }

    Mix_CloseAudio();
}