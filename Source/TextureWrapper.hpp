#pragma once
#include "raylib.h"
#include <iostream>
#include <string>

class TextureWrapper 
{
public:
    explicit TextureWrapper(const char* filePath);
    ~TextureWrapper();
    Texture2D& GetTexture();

private:
    Texture2D texture;
};