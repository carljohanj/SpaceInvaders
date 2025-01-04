#include "AudioDevice.hpp"
#include "Config.hpp"
#include <filesystem>

AudioDevice::AudioDevice()
{
    InitAudioDevice();
    soundEffect = LoadSound(Config::soundEffect.string().c_str());
    if (soundEffect.frameCount == 0)
    {
        CloseAudioDevice();
        throw std::runtime_error("Failed to load sound from file");
    }
}

AudioDevice::~AudioDevice()
{
    UnloadSound(soundEffect);
    CloseAudioDevice();
}

void AudioDevice::PlaySoundEffect() const noexcept { ::PlaySound(soundEffect); }