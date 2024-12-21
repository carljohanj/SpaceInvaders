#include "TextureWrapper2.hpp"
#include "TextureLoadingException.hpp"



Texture2D TextureWrapper2::texture = { 0 };
int TextureWrapper2::instanceCount = 0;

TextureWrapper2::TextureWrapper2(const char* filePath)
{
    if (instanceCount == 0) {
        texture = LoadTexture(filePath);
        if (texture.id == 0) {
            throw TextureLoadingException(std::string("Failed to load texture: ") + filePath);
        }
    }
    instanceCount++;
}

TextureWrapper2::~TextureWrapper2()
{
    if (instanceCount == 1 && texture.id != 0) {
        UnloadTexture(texture);
        texture = { 0 };
    }
    instanceCount--;
}


void TextureWrapper2::IncrementInstanceCount() noexcept { instanceCount++; };
void TextureWrapper2::DecrementInstanceCount() noexcept { instanceCount--; };
Texture2D& TextureWrapper2::GetTexture() noexcept { return texture; }
const Texture2D& TextureWrapper2::GetTexture() const noexcept { return texture; }