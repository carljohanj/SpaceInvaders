#pragma once
#include "raylib.h"
#include "StackManager.hpp"
#include <filesystem>
#include <unordered_map>

class TextureWrapper
{
public:
    explicit TextureWrapper(const std::filesystem::path& texturePath, int targetWidth = 0, int targetHeight = 0);
    ~TextureWrapper();
    TextureWrapper(const TextureWrapper&) = delete;
    TextureWrapper& operator=(const TextureWrapper&) = delete;
    TextureWrapper(TextureWrapper&& other) noexcept;
    TextureWrapper& operator=(TextureWrapper&& other) noexcept;
    const Texture2D& GetTexture() const;

private:
    struct Private;
    StackManager<50> impl;
};