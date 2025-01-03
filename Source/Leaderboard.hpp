#pragma once
#include <vector>
#include <string>
#include <string_view>
#include "raylib.h"

struct PlayerData 
{
    std::string name;
    int score;
};

class Leaderboard
{
public:
    Leaderboard();
    [[nodiscard]] bool HasNewHighScore(int score) const noexcept;
    [[nodiscard]] bool SaveHighScore(int score) noexcept;
    void RenderLeaderboard() const noexcept;
    void RenderHighScoreEntry(int score) const noexcept;

private:
    void InsertNewHighScore(const std::string& playerName, int score) noexcept;
    auto FindLowestScore() noexcept;
    void ResetInputState() noexcept;
    void HandleTextInput() noexcept;
    [[nodiscard]] bool TrySaveScore(int score) noexcept;
    [[nodiscard]] bool InputIsComplete() const noexcept;
    void RenderScores() const noexcept;
    void DrawScoreEntry(const std::pair<size_t, PlayerData>& entry) const noexcept;
    void RenderHeader(std::string_view text, int yOffset) const noexcept;
    void RenderFooter(std::string_view message, int yOffset) const noexcept;
    std::vector<PlayerData> scores;
    static constexpr size_t maxEntries = 5;
    std::string playerName;
    bool mouseOnText = false;
    int letterCount = 0;
    int framesCounter = 0;
    Rectangle textBox = { 600, 400, 200, 50 };
};