#pragma once
#include "raylib.h"
#include <string>
#include <iostream>

class TextureManager 
{
public:
    static Texture2D& GetTexture(const std::string& texturePath) {
        static Texture2D texture = LoadTexture(texturePath.c_str());
        if (texture.id == 0) {
            throw std::runtime_error("Failed to load texture: " + texturePath);
        }
        return texture;
    }
};