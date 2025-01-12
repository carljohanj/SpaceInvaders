#include "Star.hpp"
#include "Utilities.hpp"

void Star::Update(float starOffset) noexcept
{
    position.x = initPosition.x + starOffset;
    position.y = initPosition.y;
}

void Star::Render() const noexcept { Util::DrawCircle(position, size, color); }