#include "TextureWrapper.hpp"

TextureWrapper::TextureWrapper(const char* filePath)
{
    texture = LoadTexture(filePath);

    if (texture.id == 0) 
    {
        throw std::runtime_error(std::string("Failed to load texture: ") + filePath);
    }

    std::cout << "Texture loaded: " << filePath << std::endl;
}

TextureWrapper::~TextureWrapper() 
{
    UnloadTexture(texture);
    std::cout << "Texture unloaded." << std::endl;
}

Texture2D& TextureWrapper::GetTexture() 
{
    return texture;
}