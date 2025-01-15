#include "FileHandler.hpp"
#include <fstream>
#include <sstream>
#include <format>
#include <iostream>

struct FileHandler::Private
{
    explicit Private(std::filesystem::path path);

    [[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> LoadScores() const;
    std::expected<void, std::string> SaveScores(const std::vector<std::pair<std::string_view, int>>& scores) const;

private:
    void EnsureFileExists(std::filesystem::path path) const;
    [[nodiscard]] std::expected<std::ifstream, std::string> OpenInputFile() const;
    [[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> ParseScores(std::ifstream& inputFile) const;
    [[nodiscard]] std::expected<std::pair<std::string, int>, std::string> ParseScoreEntry(const std::string& line) const;
    [[nodiscard]] std::expected<std::ofstream, std::string> OpenOutputFile() const;
    [[nodiscard]] std::expected<void, std::string> WriteScores(std::ofstream& outputFile, const std::vector<std::pair<std::string_view, int>>& scores) const;

    std::filesystem::path filePath;
};

FileHandler::Private::Private(std::filesystem::path path)
    : filePath(std::move(path))
{
    EnsureFileExists(filePath);
}

FileHandler::FileHandler(std::filesystem::path path)
{
    impl.initialize<Private>(std::move(path));
}

[[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> FileHandler::LoadScores() const
{
    return impl.get<Private>()->LoadScores();
}

std::expected<void, std::string> FileHandler::SaveScores(const std::vector<std::pair<std::string_view, int>>& scores) const
{
    return impl.get<Private>()->SaveScores(scores);
}

[[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> FileHandler::Private::LoadScores() const
{
    auto inputFile = OpenInputFile();
    if (!inputFile) { return std::unexpected(inputFile.error()); }
    auto scores = ParseScores(*inputFile);
    if (!scores) { return std::unexpected(scores.error()); }
    return scores.value();
}

std::expected<void, std::string> FileHandler::Private::SaveScores(const std::vector<std::pair<std::string_view, int>>& scores) const
{
    auto outputFile = OpenOutputFile();
    if (!outputFile) { return std::unexpected(outputFile.error()); }
    auto writeResult = WriteScores(*outputFile, scores);
    if (!writeResult) { return std::unexpected(writeResult.error()); }
    return {};
}

void FileHandler::Private::EnsureFileExists(std::filesystem::path path) const
{
    if (!std::filesystem::exists(path))
    {
        std::ofstream file(path);
        file.close();
    }
}

[[nodiscard]] std::expected<std::ifstream, std::string> FileHandler::Private::OpenInputFile() const
{
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        return std::unexpected("Failed to open file: " + filePath.string());
    }
    return std::move(inputFile);
}

[[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> FileHandler::Private::ParseScores(std::ifstream& inputFile) const
{
    std::vector<std::pair<std::string, int>> scores;
    std::string line;
    while (std::getline(inputFile, line))
    {
        auto parsedEntry = ParseScoreEntry(line);
        if (parsedEntry) { scores.emplace_back(parsedEntry.value()); }
    }
    if (inputFile.bad())
    {
        return std::unexpected("Error reading file: " + filePath.string());
    }
    return scores;
}

[[nodiscard]] std::expected<std::pair<std::string, int>, std::string> FileHandler::Private::ParseScoreEntry(const std::string& line) const
{
    std::istringstream lineStream(line);
    std::string name;
    int score;
    if (lineStream >> name >> score)
    {
        return std::make_pair(std::move(name), score);
    }
    return std::unexpected("Failed to parse line: " + line);
}

[[nodiscard]] std::expected<std::ofstream, std::string> FileHandler::Private::OpenOutputFile() const
{
    std::ofstream outputFile(filePath, std::ios::trunc);
    if (!outputFile.is_open())
    {
        return std::unexpected("Failed to open file for writing: " + filePath.string());
    }
    return std::move(outputFile);
}

[[nodiscard]] std::expected<void, std::string> FileHandler::Private::WriteScores(std::ofstream& outputFile, const std::vector<std::pair<std::string_view, int>>& scores) const
{
    for (const auto& score : scores)
    {
        if (!(outputFile << std::format("{} {}\n", score.first, score.second)))
        {
            return std::unexpected("Error writing to file: " + filePath.string());
        }
    }
    if (outputFile.bad())
    {
        return std::unexpected("Error finalizing file write: " + filePath.string());
    }
    return {};
}