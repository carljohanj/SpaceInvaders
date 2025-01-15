#include "TextureWrapper.hpp"
#include <stdexcept>
#include <gsl/gsl>
#include "TextureLoadingException.hpp"

std::unordered_map<std::filesystem::path, TextureWrapper::TextureData> TextureWrapper::textureCache;

TextureWrapper::TextureWrapper(const std::filesystem::path& path, int targetWidth, int targetHeight)
    : texturePath(path)
{
    if   (TextureIsInCache(path)) { textureCache[path].referenceCount++; }
    else { textureCache[path] = LoadAndCacheTexture(path, targetWidth, targetHeight); }
}

[[gsl::suppress(f. 6, justification: "This will never happen; just ignore")]]
TextureWrapper::~TextureWrapper() { DecrementTextureReference(texturePath); }

TextureWrapper::TextureWrapper(TextureWrapper&& other) noexcept
    : texturePath(std::move(other.texturePath))
{
    other.texturePath.clear();
}

[[gsl::suppress(f. 6, justification: "This will never happen; just ignore")]]
TextureWrapper& TextureWrapper::operator=(TextureWrapper&& other) noexcept
{
    if (this != &other)
    {
        DecrementTextureReference(texturePath);
        texturePath = std::move(other.texturePath);
        other.texturePath.clear();
    }
    return *this;
}

const Texture2D& TextureWrapper::GetTexture() const
{
    const auto textureEntry = textureCache.find(texturePath);
    return textureEntry->second.texture;
}

void TextureWrapper::DecrementTextureReference(const std::filesystem::path& path)
{
    if (TextureIsInCache(path)) { MaybeUnload(path); }
}

[[nodiscard]] bool TextureWrapper::TextureIsInCache(const std::filesystem::path& path) const
{
    return textureCache.find(path) != textureCache.end();
}

void TextureWrapper::MaybeUnload(const std::filesystem::path& path)
{
    auto& textureData = textureCache[path];
    if (--textureData.referenceCount == 0)
    {
        UnloadTexture(textureData.texture);
        textureCache.erase(path);
    }
}

TextureWrapper::TextureData TextureWrapper::LoadAndCacheTexture(const std::filesystem::path& path, int targetWidth, int targetHeight)
{
    const Texture2D texture = LoadAndResizeTexture(path, targetWidth, targetHeight);
    if (texture.id == 0) 
    {
        throw TextureLoadingException("Failed to create texture: " + path.string());
    }
    return { texture, 1 };
}

Texture2D TextureWrapper::LoadAndResizeTexture(const std::filesystem::path& path, int targetWidth, int targetHeight)
{
    Image image = LoadImageWithValidation(path);
    ResizeImageIfNeeded(image, targetWidth, targetHeight);
    const Texture2D texture = CreateTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

inline Image TextureWrapper::LoadImageWithValidation(const std::filesystem::path& path)
{
    const Image image = LoadImage(path.string().c_str());
    if (!image.data)
    {
        throw TextureLoadingException("Failed to load image: " + path.string());
    }
    return image;
}

inline void TextureWrapper::ResizeImageIfNeeded(Image& image, int targetWidth, int targetHeight) noexcept
{
    if (targetWidth > 0 && targetHeight > 0) { ImageResize(&image, targetWidth, targetHeight); }
}

inline Texture2D TextureWrapper::CreateTextureFromImage(const Image& image)
{
    const Texture2D texture = LoadTextureFromImage(image);
    if (texture.id == 0)
    {
        throw TextureLoadingException("Failed to create texture from image.");
    }
    return texture;
}