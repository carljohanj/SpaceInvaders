#pragma once
#include "raylib.h"

class Wall 
{
public:
    Vector2 position = { 0, 0 };
    Rectangle rec = { 0, 0, 0, 0 };
    bool active = true;
    Color color = GRAY;
    int health = 50;
    int radius = 60;

    void Update();
    void Render(Texture2D texture);
};
