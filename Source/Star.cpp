#include "Star.hpp"

void Star::Update(float starOffset) noexcept
{
    position.x = initPosition.x + starOffset;
    position.y = initPosition.y;
}

void Star::Render() const noexcept
{
    DrawCircle((int)position.x, (int)position.y, size, color);
}
