#include "TextureWrapper.hpp"
#include "TextureLoadingException.hpp"
#include <stdexcept>
#include <utility>
#include <gsl/gsl>

struct TextureWrapper::Private
{
    struct TextureData
    {
        Texture2D texture;
        int referenceCount;
    };

    std::filesystem::path texturePath;
    static std::unordered_map<std::filesystem::path, TextureData> textureCache;

    Private(const std::filesystem::path& path, int targetWidth, int targetHeight);
    ~Private();
    void DecrementTextureReference(const std::filesystem::path& path);
    [[nodiscard]] bool TextureIsInCache(const std::filesystem::path& path) const;
    void MaybeUnload(const std::filesystem::path& path);

    static TextureData LoadAndCacheTexture(const std::filesystem::path& path, int targetWidth, int targetHeight);
    static Texture2D LoadAndResizeTexture(const std::filesystem::path& path, int targetWidth, int targetHeight);
    static Image LoadImageWithValidation(const std::filesystem::path& path);
    static void ResizeImageIfNeeded(Image& image, int targetWidth, int targetHeight) noexcept;
    static Texture2D CreateTextureFromImage(const Image& image);
};

std::unordered_map<std::filesystem::path, TextureWrapper::Private::TextureData> TextureWrapper::Private::textureCache;

TextureWrapper::TextureWrapper(const std::filesystem::path& texturePath, int targetWidth, int targetHeight)
{
    impl.initialize<Private>(texturePath, targetWidth, targetHeight);
}

TextureWrapper::~TextureWrapper() = default;

TextureWrapper::TextureWrapper(TextureWrapper&& other) noexcept
    : impl(std::move(other.impl))
{
}

TextureWrapper& TextureWrapper::operator=(TextureWrapper&& other) noexcept
{
    if (this != &other)
    {
        impl = std::move(other.impl);
    }
    return *this;
}

const Texture2D& TextureWrapper::GetTexture() const
{
    auto* pImpl = impl.get<Private>();
    return Private::textureCache[pImpl->texturePath].texture;
}

TextureWrapper::Private::Private(const std::filesystem::path& path, int targetWidth, int targetHeight)
    : texturePath(path)
{
    if (TextureIsInCache(path))
    {
        textureCache[path].referenceCount++;
    }
    else
    {
        textureCache[path] = LoadAndCacheTexture(path, targetWidth, targetHeight);
    }
}

[[gsl::suppress(f .6, justification: "This will never happen; just ignore")]]
[[gsl::suppress(c .21, justification: "TextureWrapper already has defined special members")]]
TextureWrapper::Private::~Private() { DecrementTextureReference(texturePath); }

void TextureWrapper::Private::DecrementTextureReference(const std::filesystem::path& path)
{
    if (TextureIsInCache(path))
    {
        MaybeUnload(path);
    }
}

[[nodiscard]] bool TextureWrapper::Private::TextureIsInCache(const std::filesystem::path& path) const
{
    return textureCache.find(path) != textureCache.end();
}

void TextureWrapper::Private::MaybeUnload(const std::filesystem::path& path)
{
    auto& textureData = textureCache[path];
    if (--textureData.referenceCount == 0)
    {
        UnloadTexture(textureData.texture);
        textureCache.erase(path);
    }
}

TextureWrapper::Private::TextureData TextureWrapper::Private::LoadAndCacheTexture(const std::filesystem::path& path, int targetWidth, int targetHeight)
{
    const Texture2D texture = LoadAndResizeTexture(path, targetWidth, targetHeight);
    if (texture.id == 0)
    {
        throw TextureLoadingException("Failed to create texture: " + path.string());
    }
    return { texture, 1 };
}

Texture2D TextureWrapper::Private::LoadAndResizeTexture(const std::filesystem::path& path, int targetWidth, int targetHeight)
{
    Image image = LoadImageWithValidation(path);
    ResizeImageIfNeeded(image, targetWidth, targetHeight);
    const Texture2D texture = CreateTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

Image TextureWrapper::Private::LoadImageWithValidation(const std::filesystem::path& path)
{
    const Image image = LoadImage(path.string().c_str());
    if (!image.data)
    {
        throw TextureLoadingException("Failed to load image: " + path.string());
    }
    return image;
}

void TextureWrapper::Private::ResizeImageIfNeeded(Image& image, int targetWidth, int targetHeight) noexcept
{
    if (targetWidth > 0 && targetHeight > 0)
    {
        ImageResize(&image, targetWidth, targetHeight);
    }
}

Texture2D TextureWrapper::Private::CreateTextureFromImage(const Image& image)
{
    const Texture2D texture = LoadTextureFromImage(image);
    if (texture.id == 0)
    {
        throw TextureLoadingException("Failed to create texture from image.");
    }
    return texture;
}