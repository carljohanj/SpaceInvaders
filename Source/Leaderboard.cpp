#include "Leaderboard.hpp"
#include "Config.hpp"
#include "raylib.h"
#include <algorithm>
#include <ranges>
#include <string_view>

inline constexpr int textBoxX = 600;
inline constexpr int textBoxY = 500;
inline constexpr int textBoxWidth = 300;
inline constexpr int textBoxHeight = 50;
inline constexpr int leaderboardFontSize = 40;
inline constexpr int bigYOffset = 200;
inline constexpr int smallYOffset = 100;
inline constexpr int highScoreFontSize = 60;
inline constexpr int maxNameLength = 9;
inline constexpr int blinkTimerReset = 0;
inline constexpr std::string_view enterMessage = "PRESS ENTER TO CONTINUE";
inline constexpr std::string_view leaderboardHeader = "CURRENT LEADERBOARD";
inline constexpr std::string_view highScoreHeader = "NEW HIGHSCORE!";

inline constexpr ScoreRendering drawScoresTopLeftCorner
{
    .startX = 50,
    .currentY = 100,
    .ySpacing = (leaderboardFontSize - 10) + 5,
    .fontSize = leaderboardFontSize - 10
};

Leaderboard::Leaderboard()
    : fileHandler(Config::leaderBoardScores),
      textBox({ textBoxX, textBoxY, textBoxWidth, textBoxHeight })
{
    LoadScoresFromFile();
}

[[nodiscard]] bool Leaderboard::HasNewHighScore(int score) const noexcept { return score > scores.back().score; }

[[nodiscard]] bool Leaderboard::SaveHighScore(int score) noexcept
{
    CapturePlayerNameInput();
    return TrySaveScore(score);
}

void Leaderboard::CapturePlayerNameInput() noexcept
{
    const int key = GetCharPressed();
    if ((key >= 32) && (key <= 125) && playerName.size() < maxNameLength)
    {
        playerName.push_back(static_cast<char>(key));
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) { playerName.pop_back(); }
    blinkTimer++;
}

[[nodiscard]] inline bool Leaderboard::TrySaveScore(int score) noexcept
{
    if (!InputIsComplete()) { return false; }
    InsertNewHighScore(playerName, score);
    ResetInputState();
    return true;
}

[[nodiscard]] inline bool Leaderboard::InputIsComplete() const noexcept
{
    return !playerName.empty() && IsKeyReleased(KEY_ENTER);
}

[[nodiscard]] inline auto Leaderboard::FindLowestScore() noexcept
{
    return std::ranges::min_element(scores, [](const PlayerData& a, const PlayerData& b)
    {
        return a.score < b.score;
    });
}

void Leaderboard::InsertNewHighScore(const std::string& name, int score)
{
    auto lowestScore = FindLowestScore();
    if (score > lowestScore->score) { *lowestScore = { name, score }; }
    std::ranges::sort(scores, [](const PlayerData& a, const PlayerData& b) { return a.score > b.score; });
    SaveScoresToFile();
}


inline void Leaderboard::ResetInputState() noexcept
{
    playerName.clear();
    blinkTimer = blinkTimerReset;
}

void Leaderboard::RenderLeaderboard() const noexcept
{
    RenderHeader(leaderboardHeader, smallYOffset);
    RenderScores(drawScoresTopLeftCorner);
    RenderFooter(enterMessage, scores.size() * (leaderboardFontSize + 10) + 50);
}

inline void Leaderboard::RenderScores(ScoreRendering screenPos) const noexcept
{
    std::ranges::for_each(scores, [&](const PlayerData& data) noexcept
    {
        DrawScoreEntry(data, screenPos);
        screenPos.currentY += screenPos.ySpacing;
    });
}

inline void Leaderboard::DrawScoreEntry(const PlayerData& data, const ScoreRendering& screenPos) const noexcept
{
    DrawText(data.name.c_str(), screenPos.startX, screenPos.currentY, screenPos.fontSize, YELLOW);
    DrawText(TextFormat("%d", data.score), screenPos.startX + 300, screenPos.currentY, screenPos.fontSize, YELLOW);
}

inline void Leaderboard::RenderHeader(std::string_view text, int yOffset) const noexcept
{
    DrawText(text.data(), textBoxX, textBoxY + yOffset, leaderboardFontSize, YELLOW);
}

inline void Leaderboard::RenderFooter(std::string_view message, int yOffset) const noexcept
{
    DrawText(message.data(), textBoxX, textBoxY + yOffset, leaderboardFontSize, YELLOW);
}

void Leaderboard::RenderHighScoreEntry() noexcept
{
    RenderHeader(highScoreHeader, bigYOffset);
    RenderScores(drawScoresTopLeftCorner);
    RenderTextBox();
    RenderNameInput();
    if (!playerName.empty()) { RenderFooter(enterMessage, smallYOffset); }
}

inline void Leaderboard::RenderTextBox() const noexcept
{
    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, RED);
}

inline void Leaderboard::RenderNameInput() noexcept
{
    RenderPlayerTextInput();
    if (ShouldRenderCursor()) { RenderBlinkingCursor(); }
}

inline void Leaderboard::RenderPlayerTextInput() const noexcept
{
    DrawText(playerName.c_str(), static_cast<int>(textBox.x) + 5,
        static_cast<int>(textBox.y) + 8, leaderboardFontSize, MAROON);
}

[[nodiscard]] inline bool Leaderboard::ShouldRenderCursor() const noexcept
{
    return playerName.size() < maxNameLength && ((blinkTimer / 20) % 2) == 0;
}

inline void Leaderboard::RenderBlinkingCursor() const noexcept
{
    DrawText("_", static_cast<int>(textBox.x) + 8 + MeasureText(playerName.c_str(), leaderboardFontSize),
             static_cast<int>(textBox.y) + 12, leaderboardFontSize, MAROON);
}

void Leaderboard::LoadScoresFromFile()
{
    auto fileContent = fileHandler.LoadScores();
    if (!fileContent) { return; }
    scores.clear();
    std::ranges::transform(fileContent.value(), std::back_inserter(scores), [](const auto& pair) 
    {
        return PlayerData{ pair.first, pair.second };
    });
}

void Leaderboard::SaveScoresToFile()
{
    std::vector<std::pair<std::string_view, int>> scoreViews;
    scoreViews.reserve(scores.size());
    std::ranges::transform(scores, std::back_inserter(scoreViews), [](const PlayerData& data) 
    {
        return std::make_pair(std::string_view(data.name), data.score);
    });
    fileHandler.SaveScores(scoreViews);
}