#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <filesystem>
#include <optional>

class FileHandler 
{
public:
    explicit FileHandler(std::filesystem::path filePath);
    [[nodiscard]] bool FileExists() const noexcept;
    [[nodiscard]] std::optional<std::vector<std::pair<std::string, int>>> LoadScores() const noexcept;
    bool SaveScores(std::vector<std::pair<std::string_view, int>> scores) noexcept;

private:
    std::filesystem::path filePath;
    void EnsureFileExists() const noexcept;
};