#include "AudioDevice.hpp"
#include "Config.hpp"

static const std::unordered_map<SoundEffect, std::filesystem::path> SoundFilePaths = 
{
    { SoundEffect::IsHit, Config::hitSound },
    { SoundEffect::PlayerShoots, Config::playerShot },
    { SoundEffect::AlienShoots, Config::alienShot },
    { SoundEffect::StartSound, Config::startGame },
    { SoundEffect::NewHighScore, Config::highScore },
    { SoundEffect::PlayerDies, Config::dead },
    { SoundEffect::BackgroundMusic, Config::backgroundMusic }
};

AudioDevice::AudioDevice()
{
    InitAudioDevice();
    LoadAllSounds();
    LoadMusic();
}

AudioDevice::~AudioDevice() noexcept
{
    UnloadAllSounds();
    UnloadMusic();
    CloseAudioDevice();
}

[[gsl::suppress(f .6, justification: "This will not be out of bounds")]]
void AudioDevice::Play(SoundEffect sound) noexcept { PlaySound(sounds[sound]); }

void AudioDevice::UpdateMusic() const noexcept { UpdateMusicStream(backgroundMusic);}

void AudioDevice::PlayBackgroundMusic() const noexcept { PlayMusicStream(backgroundMusic); }

void AudioDevice::StopBackgroundMusic() const noexcept { StopMusicStream(backgroundMusic); }

void AudioDevice::LoadAllSounds() noexcept
{
    for (const auto& [effect, filePath] : SoundFilePaths)
    {
        if (effect == SoundEffect::BackgroundMusic) { continue; }
        Sound sound = LoadSound(filePath.string().c_str());
        sounds.emplace(effect, sound);
    }
}

void AudioDevice::UnloadAllSounds() noexcept
{
    for (const auto& [effect, sound] : sounds)
    {
        UnloadSound(sound);
    }
    sounds.clear();
}

void AudioDevice::LoadMusic() noexcept
{
    auto it = SoundFilePaths.find(SoundEffect::BackgroundMusic);
    if (it != SoundFilePaths.end())
    {
        backgroundMusic = LoadMusicStream(it->second.string().c_str());
    }
}

void AudioDevice::UnloadMusic() const noexcept
{
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);
}