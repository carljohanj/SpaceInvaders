#pragma once
#include "raylib.h"
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
    struct TextureData
    {
        Texture2D texture;
        int referenceCount;
    };

    std::filesystem::path texturePath;
    static std::unordered_map<std::filesystem::path, TextureData> textureCache;
    static TextureData LoadAndCacheTexture(const std::filesystem::path& path, int targetWidth, int targetHeight);
    static Texture2D LoadAndResizeTexture(const std::filesystem::path& path, int targetWidth, int targetHeight);
    static Image LoadImageWithValidation(const std::filesystem::path& path);
    static void ResizeImageIfNeeded(Image& image, int targetWidth, int targetHeight) noexcept;
    static Texture2D CreateTextureFromImage(const Image& image);
    void DecrementTextureReference(const std::filesystem::path& path);
    [[nodiscard]] bool TextureIsInCache(const std::filesystem::path& path) const;
    void MaybeUnload(const std::filesystem::path& path);
};