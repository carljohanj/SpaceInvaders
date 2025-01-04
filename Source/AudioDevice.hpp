#pragma once
#include <filesystem>
#include "raylib.h"
#include <string>
#include <stdexcept>

class AudioDevice
{
public:
    AudioDevice();
    ~AudioDevice();
    void PlaySoundEffect() const noexcept;

private:
    Sound soundEffect;
};