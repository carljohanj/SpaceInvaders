#include "TextureWrapper.hpp"
#include <stdexcept>
#include <iostream>
#include "TextureLoadingException.hpp"

std::unordered_map<std::filesystem::path, TextureWrapper::TextureData> TextureWrapper::textureCache;

TextureWrapper::TextureWrapper(const std::filesystem::path& path)
    : texturePath(path)
{
    if (TextureIsInCache(path)) 
    {
        textureCache[path].referenceCount++;
    }
    else
    {
        const Texture2D texture = LoadTexture(path.string().c_str());
        if (texture.id == 0)
        {
            throw TextureLoadingException("Failed to load " + path.string());
        }
        textureCache[path] = { texture, 1 };
    }
}

TextureWrapper::~TextureWrapper() { DecrementTextureReference(texturePath); }

TextureWrapper::TextureWrapper(TextureWrapper&& other) noexcept
    : texturePath(std::move(other.texturePath))
{
    other.texturePath.clear();
}

TextureWrapper& TextureWrapper::operator=(TextureWrapper&& other) noexcept
{
    DecrementTextureReference(texturePath);
    texturePath = std::move(other.texturePath);
    other.texturePath.clear();
    return *this;
}

const Texture2D& TextureWrapper::GetTexture() const noexcept
{
    const auto textureEntry = textureCache.find(texturePath);
    return textureEntry->second.texture;
}

void TextureWrapper::DecrementTextureReference(const std::filesystem::path& path) noexcept
{
    if (TextureIsInCache(path)) { MaybeUnload(path); }
}

inline bool TextureWrapper::TextureIsInCache(const std::filesystem::path& path) const
{
    return textureCache.find(path) != textureCache.end();
}

inline void TextureWrapper::MaybeUnload(const std::filesystem::path& path) noexcept
{
    auto& textureData = textureCache[path];
    textureData.referenceCount--;
    if (textureData.referenceCount == 0)
    {
        UnloadTexture(textureData.texture);
        textureCache.erase(path);
    }
}