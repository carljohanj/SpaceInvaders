#include "TextureWrapper.hpp"
#include <stdexcept>
#include <iostream>

std::unordered_map<std::string, std::pair<Texture2D, int>> TextureWrapper::textureCache;

TextureWrapper::TextureWrapper(const std::string_view texturePath)
    : texturePath(texturePath)
{
    IncrementReference(texturePath.data());
}

TextureWrapper::~TextureWrapper()
{
    DecrementReference(texturePath);
}

TextureWrapper::TextureWrapper(TextureWrapper&& other) noexcept
    : texturePath(std::move(other.texturePath))
{
    other.texturePath.clear();
}

TextureWrapper& TextureWrapper::operator=(TextureWrapper&& other) noexcept
{
    if (this != &other) 
    {
        DecrementReference(texturePath);
        texturePath = std::move(other.texturePath);
        other.texturePath.clear();
    }
    return *this;
}

const Texture2D& TextureWrapper::GetTexture() const noexcept
{
    return textureCache.at(texturePath).first;
}

void TextureWrapper::IncrementReference(const std::string& path)
{
    auto it = textureCache.find(path);
    if (it != textureCache.end()) { it->second.second++; }
    else 
    {
        Texture2D texture = LoadTexture(path.c_str());
        if (texture.id == 0) 
        {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        textureCache[path] = { texture, 1 };
        std::cout << "Texture loaded: " << path << std::endl;
    }
}

void TextureWrapper::DecrementReference(const std::string& path)
{
    auto it = textureCache.find(path);
    if (it != textureCache.end()) {
        // Decrement reference count
        it->second.second--;

        // If no more references, unload the texture
        if (it->second.second == 0) {
            UnloadTexture(it->second.first);
            textureCache.erase(it);
            std::cout << "Texture unloaded: " << path << std::endl;
        }
    }
}