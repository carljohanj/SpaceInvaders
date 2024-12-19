#pragma once
#include "raylib.h"
#include <memory>
#include <iostream>
#include <string>

class TextureWrapper 
{
public:
    explicit TextureWrapper(const char* filePath);
    Texture2D& GetTexture();

private:
    std::unique_ptr<Texture2D, void(*)(Texture2D*)> texturePtr;
};