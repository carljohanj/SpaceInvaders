#pragma once
#include "raylib.h"
#include "SoundEffects.hpp"
#include <unordered_map>
#include <filesystem>

class AudioDevice
{
public:
    AudioDevice();
    ~AudioDevice() noexcept;

    void Play(SoundEffect sound) noexcept;
    void UpdateMusic() const noexcept;
    void PlayBackgroundMusic() const noexcept;
    void StopBackgroundMusic() const noexcept;

private:
    std::unordered_map<SoundEffect, Sound> sounds;
    Music backgroundMusic;

    void LoadAllSounds() noexcept;
    void UnloadAllSounds() noexcept;
    void LoadMusic() noexcept;
    void UnloadMusic() const noexcept;
};