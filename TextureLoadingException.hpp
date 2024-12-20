#pragma once
#include <stdexcept>
#include <string>
#include <string_view>

class TextureLoadingException final : public std::runtime_error
{
public:
    explicit TextureLoadingException(std::string_view message)
        : std::runtime_error(std::string("Error loading texture: ") + message.data()) {}
};