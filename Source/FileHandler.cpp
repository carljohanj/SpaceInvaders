#include "FileHandler.hpp"
#include <fstream>
#include <sstream>
#include <ranges>
#include <format>
#include <iostream>

FileHandler::FileHandler(std::filesystem::path filePath)
    : filePath(std::move(filePath))
{
    auto result = EnsureFileExists();
    if (!result) 
    {
        std::cerr << "Error ensuring file exists: " << result.error() << '\n';
    }
}

[[nodiscard]] bool FileHandler::FileExists() const noexcept { return std::filesystem::exists(filePath); }

[[nodiscard]] std::expected<void, std::string> FileHandler::EnsureFileExists() const noexcept 
{
    if (!FileExists()) { return; }
    std::ofstream createFile(filePath, std::ios::out);
    if (!createFile.is_open()) 
    {
        return std::unexpected("Failed to create file at " + filePath.string());
    }
    createFile.close();
    return {};
}

[[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> FileHandler::LoadScores() const noexcept {
    if (!FileExists()) {
        return std::unexpected("File does not exist: " + filePath.string());
    }

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        return std::unexpected("Failed to open file: " + filePath.string());
    }

    std::vector<std::pair<std::string, int>> scores;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream lineStream(line);
        std::string name;
        int score;

        if (lineStream >> name >> score) {
            scores.emplace_back(std::move(name), score);
        }
    }

    inputFile.close();
    return scores;
}

[[nodiscard]] std::expected<void, std::string> FileHandler::SaveScores(const std::vector<std::pair<std::string_view, int>>& scores) noexcept {
    std::ofstream outputFile(filePath, std::ios::trunc); // Truncate file to overwrite
    if (!outputFile.is_open()) {
        return std::unexpected("Failed to open file for writing: " + filePath.string());
    }

    for (const auto& [name, score] : scores) {
        outputFile << std::format("{} {}\n", name, score);
    }

    outputFile.close();
    return {};
}