#pragma once
#include "raylib.h"
#include <iostream>
#include <string>

class TextureWrapper2
{
public:
    explicit TextureWrapper2(const char* filePath);
    ~TextureWrapper2();
    Texture2D& GetTexture() noexcept;
    const Texture2D& GetTexture() const noexcept;
    void IncrementInstanceCount() noexcept;
    void DecrementInstanceCount() noexcept;

private:
    static Texture2D texture;
    static int instanceCount;
};