#include "Background.hpp"
#include <algorithm>
#include "Config.hpp"
#include <random>
#include <ranges>

Background::Background()
    : Stars(),
      starAmount(Config::backgroundStarAmount),
      xMinOffset(Config::backgroundXMinOffset),
      xMaxOffset(Config::backgroundXMaxOffset),
      bgSpeed(Config::backgroundSpeed)
{
    Stars.reserve(starAmount);
    for ([[maybe_unused]] auto i : std::views::iota(0, starAmount))
    {
        Star newStar;
        newStar.initPosition.x = GetRandomValue(xMinOffset, GetScreenWidth() + xMaxOffset);
        newStar.initPosition.y = GetRandomValue(0, GetScreenHeight());
        newStar.color = SKYBLUE;
        newStar.size = GetRandomValue(1, 4) / 2.0f;
        Stars.push_back(std::move(newStar));
    }
}

void Background::Reset() noexcept
{
    Stars.clear();
    Stars.reserve(starAmount);
    for ([[maybe_unused]] auto i : std::views::iota(0, starAmount))
    {
        Star newStar;
        newStar.initPosition.x = GetRandomValue(xMinOffset, GetScreenWidth() + xMaxOffset);
        newStar.initPosition.y = GetRandomValue(0, GetScreenHeight());
        newStar.color = SKYBLUE;
        newStar.size = GetRandomValue(1, 4) / 2.0f;
        Stars.push_back(std::move(newStar));
    }
}

void Background::Update(float playerOffset) noexcept
{
    for (auto& star : Stars | std::views::all)
    {
        star.Update(playerOffset);
    }
}

void Background::Render() const noexcept
{
    for (const auto& star : Stars)
    {
        star.Render();
    }
}