#pragma once
#include "StackManager.hpp"
#include <filesystem>
#include <string>
#include <string_view>
#include <expected>

class FileHandler
{
public:
    explicit FileHandler(std::filesystem::path path);
    [[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> LoadScores() const;
    std::expected<void, std::string> SaveScores(const std::vector<std::pair<std::string_view, int>>& scores) const;

private:
    struct Private;
    StackManager<50> impl;
};