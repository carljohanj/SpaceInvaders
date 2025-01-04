#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <filesystem>
#include <optional>
#include <expected>

class FileHandler 
{
public:
    explicit FileHandler(std::filesystem::path filePath);

    [[nodiscard]] bool FileExists() const noexcept;
    [[nodiscard]] std::expected<void, std::string> EnsureFileExists() const noexcept;
    [[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> LoadScores() const noexcept;
    [[nodiscard]] std::expected<void, std::string> SaveScores(const std::vector<std::pair<std::string_view, int>>& scores) noexcept;

private:
    std::filesystem::path filePath;
};