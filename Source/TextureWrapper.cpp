#include "TextureWrapper.hpp"

TextureWrapper::TextureWrapper(const char* filePath)
    : texturePtr(
        new Texture2D(LoadTexture(filePath)),
        [](Texture2D* texture) 
        {
            if (texture && texture->id != 0) 
            {
                UnloadTexture(*texture);
                std::cout << "Texture unloaded." << std::endl;
            }
        }) {
    if (texturePtr->id == 0) {
        throw std::runtime_error(std::string("Failed to load texture: ") + filePath);
    }
    std::cout << "Texture loaded: " << filePath << std::endl;
}

Texture2D& TextureWrapper::GetTexture() 
{
    return *texturePtr;
}