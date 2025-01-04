#include "FileHandler.hpp"
#include <fstream>
#include <sstream>
#include <ranges>
#include <format>
#include <iostream>

FileHandler::FileHandler(std::filesystem::path filePath)
    : filePath(std::move(filePath))
{
    EnsureFileExists();
}

[[nodiscard]] bool FileHandler::FileExists() const noexcept { return std::filesystem::exists(filePath); }

void FileHandler::EnsureFileExists() const noexcept 
{
    if (FileExists()) { return; }

    std::ofstream createFile(filePath, std::ios::out);
    if (createFile.is_open()) { createFile.close(); }
}

[[nodiscard]] std::optional<std::vector<std::pair<std::string, int>>> FileHandler::LoadScores() const noexcept 
{
    if (!FileExists()) return std::nullopt;

    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) { return std::nullopt; }

    std::vector<std::pair<std::string, int>> scores;
    std::string line;

    while (std::getline(inputFile, line)) 
    {
        std::istringstream lineStream(line);
        std::string name;
        int score;

        if (lineStream >> name >> score) 
        {
            scores.emplace_back(std::move(name), score);
        }
    }

    inputFile.close();
    return scores;
}

bool FileHandler::SaveScores(std::vector<std::pair<std::string_view, int>> scores) noexcept 
{
    std::ofstream outputFile(filePath, std::ios::trunc); // Truncate file to overwrite
    if (!outputFile.is_open()) return false;

    for (const auto& [name, score] : scores) 
    {
        outputFile << std::format("{} {}\n", name, score);
    }

    outputFile.close();
    return true;
}