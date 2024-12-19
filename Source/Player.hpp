#pragma once
#include "raylib.h"
#include "Projectile.hpp"

class Player {
public:
    float x_pos = 0;
    float speed = 7;
    float player_base_height = 70.0f;
    float radius = 50;
    int lives = 3;
    int activeTexture = 0;

    void Initialize();
    void Update();
    void Render(Texture2D texture);

    Projectile Shoot();
};