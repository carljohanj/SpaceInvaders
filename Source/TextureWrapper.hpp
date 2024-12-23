#pragma once
#include "raylib.h"
#include <string>
#include <stdexcept>

class TextureWrapper 
{
public:
    explicit TextureWrapper(const std::string_view texturePath);
    ~TextureWrapper();

    TextureWrapper(const TextureWrapper&) = delete;
    TextureWrapper& operator=(const TextureWrapper&) = delete;

    TextureWrapper(TextureWrapper&& other) noexcept;
    TextureWrapper& operator=(TextureWrapper&& other) noexcept;

    const Texture2D& GetTexture() const noexcept;

private:
    static Texture2D texture;
    static int instanceCount;
};