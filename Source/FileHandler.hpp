#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <string_view>
#include <expected>

class FileHandler
{
public:
    explicit FileHandler(std::filesystem::path path);
    [[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string> LoadScores() const;
    [[nodiscard]] std::expected<void, std::string> SaveScores(const std::vector<std::pair<std::string_view, int>>& scores) const;

private:
    std::filesystem::path filePath;
    void EnsureFileExists(std::filesystem::path path) const;
    std::expected<std::ifstream, std::string> OpenInputFile() const;
    std::expected<std::vector<std::pair<std::string, int>>, std::string> ParseScores(std::ifstream& inputFile) const;
    std::expected<std::pair<std::string, int>, std::string> ParseScoreEntry(const std::string& line) const;
    std::expected<std::ofstream, std::string> OpenOutputFile() const;
    std::expected<void, std::string> WriteScores(std::ofstream& outputFile,
        const std::vector<std::pair<std::string_view, int>>& scores) const;


};