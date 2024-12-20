#pragma once
#include <stdexcept>
#include <string>
#include <string_view>

class GameWindowException final : public std::runtime_error
{
public:
    explicit GameWindowException(std::string_view message)
        : std::runtime_error(std::string("Error initializing game window: ") + message.data()) {}
};