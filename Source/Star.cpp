#include "Star.hpp"

void Star::Update(float starOffset) noexcept
{
    position.x = initPosition.x + starOffset;
    position.y = initPosition.y;
}

void Star::Render() const noexcept
{
    DrawCircle(position.x, position.y, size, color);
}
