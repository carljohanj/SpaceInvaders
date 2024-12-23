#include "TextureWrapper.hpp"
#include "TextureLoadingException.hpp"
#include <print>

Texture2D TextureWrapper::texture = { 0 };
int TextureWrapper::instanceCount = 0;

TextureWrapper::TextureWrapper(std::string_view filePath)
{
    texture = LoadTexture(filePath.data());

    if (texture.id == 0) 
    {
        throw TextureLoadingException(std::string("Failed to load texture: ").append(filePath));
    }
    instanceCount++;
    std::println("Texture loaded: {}", filePath);
}

TextureWrapper::~TextureWrapper()
{
    instanceCount--;
    if (instanceCount == 0 && texture.id != 0)
    {
        UnloadTexture(texture);
        texture = { 0 };
        std::println("Texture unloaded");
    }
}

TextureWrapper::TextureWrapper(TextureWrapper&& other) noexcept 
{
    instanceCount++;
    std::println("TextureWrapper moved! Instance count: {}", instanceCount);
}

TextureWrapper& TextureWrapper::operator=(TextureWrapper&& other) noexcept 
{
    if (this != &other) 
    {
        std::println("TextureWrapper move-assigned! Instance count remains: {}", instanceCount);
    }
    return *this;
}

const Texture2D& TextureWrapper::GetTexture() const noexcept
{
    return texture;
}