#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

class TextureWrapper {
public:
    explicit TextureWrapper(const std::string_view texturePath);
    ~TextureWrapper();

    TextureWrapper(const TextureWrapper&) = delete;
    TextureWrapper& operator=(const TextureWrapper&) = delete;

    TextureWrapper(TextureWrapper&& other) noexcept;
    TextureWrapper& operator=(TextureWrapper&& other) noexcept;

    const Texture2D& GetTexture() const noexcept;

private:
    std::string texturePath;

    static std::unordered_map<std::string, std::pair<Texture2D, int>> textureCache;
    void IncrementReference(const std::string& path);
    void DecrementReference(const std::string& path);
};