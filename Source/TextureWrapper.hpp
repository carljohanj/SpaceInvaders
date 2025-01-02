#pragma once
#include "raylib.h"
#include <filesystem>
#include <unordered_map>

class TextureWrapper
{
public:
    explicit TextureWrapper(const std::filesystem::path& texturePath);
    ~TextureWrapper();
    TextureWrapper(const TextureWrapper&) = delete;
    TextureWrapper& operator=(const TextureWrapper&) = delete;
    TextureWrapper(TextureWrapper&& other) noexcept;
    TextureWrapper& operator=(TextureWrapper&& other) noexcept;
    const Texture2D& GetTexture() const;

private:
    struct TextureData
    {
        Texture2D texture;
        int referenceCount;
    };

    std::filesystem::path texturePath;
    static std::unordered_map<std::filesystem::path, TextureData> textureCache;
    void DecrementTextureReference(const std::filesystem::path& path) noexcept;
    bool TextureIsInCache(const std::filesystem::path& path) const;
    void MaybeUnload(const std::filesystem::path& path) noexcept;
};