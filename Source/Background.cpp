#include "Background.hpp"

void Background::Initialize(int starAmount) 
{
    for (int i = 0; i < starAmount; i++) {
        Star newStar;
        newStar.initPosition.x = GetRandomValue(-150, GetScreenWidth() + 150);
        newStar.initPosition.y = GetRandomValue(0, GetScreenHeight());
        newStar.color = SKYBLUE;
        newStar.size = GetRandomValue(1, 4) / 2;
        Stars.push_back(newStar);
    }
}

void Background::Update(float offset) 
{
    for (auto& star : Stars) {
        star.Update(offset);
    }
}

void Background::Render() 
{
    for (const auto& star : Stars) {
        star.Render();
    }
}