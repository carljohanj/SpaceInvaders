#pragma once
#include "FileHandler.hpp"
#include <vector>
#include <string>
#include <string_view>
#include "raylib.h"

struct PlayerData
{
    std::string name;
    int score;
};

struct ScoreRendering
{
    int startX;
    int currentY;
    int ySpacing;
    int fontSize;
};

class Leaderboard
{
public:
    Leaderboard();
    [[nodiscard]] bool HasNewHighScore(int score) const noexcept;
    [[nodiscard]] bool SaveHighScore(int score) noexcept;
    void RenderLeaderboard() const noexcept;
    void RenderHighScoreEntry() noexcept;

private:
    void InsertNewHighScore(const std::string& playerName, int score);
    auto FindLowestScore() noexcept;
    void ResetInputState() noexcept;
    void CapturePlayerNameInput() noexcept;
    [[nodiscard]] bool TrySaveScore(int score) noexcept;
    [[nodiscard]] bool InputIsComplete() const noexcept;
    void DrawScoreEntry(const PlayerData& data, const ScoreRendering& config) const noexcept;
    void RenderHeader(std::string_view text, int yOffset) const noexcept;
    void RenderFooter(std::string_view message, int yOffset) const noexcept;
    void RenderTextBox() const noexcept;
    void RenderNameInput() noexcept;
    void RenderPlayerTextInput() const noexcept;
    void RenderScores(ScoreRendering config) const noexcept;
    [[nodiscard]] bool ShouldRenderCursor() const noexcept;
    void RenderBlinkingCursor() const noexcept;
    void LoadScoresFromFile();
    void SaveScoresToFile();
    std::vector<PlayerData> scores;
    static constexpr size_t maxEntries = 5;
    std::string playerName;
    bool mouseOnText = false;
    int letterCount = 0;
    int blinkTimer = 0;
    Rectangle textBox = { 600, 400, 200, 50 };
    FileHandler fileHandler;
};