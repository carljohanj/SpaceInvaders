#include "Leaderboard.hpp"
#include "Config.hpp"
#include "raylib.h"
#include <algorithm>
#include <ranges>
#include <string_view>

inline constexpr int textBoxX = 600.0f;
inline constexpr int textBoxY = 500.0f;
inline constexpr int textBoxWidth = 300.0f;
inline constexpr int textBoxHeight = 50.0f;
inline constexpr int leaderboardFontSize = 40;
inline constexpr int highScoreFontSize = 60;
inline constexpr int maxNameLength = 9;
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
    : scores
    {
      {"Player 1", 500},
      {"Player 2", 400},
      {"Player 3", 300},
      {"Player 4", 200},
      {"Player 5", 100}
    },
    textBox({ textBoxX, textBoxY, textBoxWidth, textBoxHeight }){
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
    if (InputIsComplete())
    {
        InsertNewHighScore(playerName, score);
        ResetInputState();
        return true;
    }
    return false;
}

[[nodiscard]] inline bool Leaderboard::InputIsComplete() const noexcept
{
    return !playerName.empty() && IsKeyReleased(KEY_ENTER);
}

[[nodiscard]] inline auto Leaderboard::FindLowestScore() noexcept
{
    return std::ranges::min_element(scores, [](const PlayerData& a, const PlayerData& b) {
        return a.score < b.score;
        });
}

void Leaderboard::InsertNewHighScore(const std::string& name, int score) noexcept
{
    auto lowestScore = FindLowestScore();
    if (score > lowestScore->score) { *lowestScore = { name, score }; }
    std::ranges::sort(scores, [] (const PlayerData& a, const PlayerData& b) { return a.score > b.score; });
}

inline void Leaderboard::ResetInputState() noexcept
{
    playerName.clear();
    blinkTimer = 0;
}

void Leaderboard::RenderLeaderboard() const noexcept
{
    RenderHeader(leaderboardHeader, -100);
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

void Leaderboard::RenderHighScoreEntry(int score) noexcept
{
    RenderHeader(highScoreHeader, -200);
    RenderScores(drawScoresTopLeftCorner);
    RenderTextBox();
    RenderNameInput();
    if (!playerName.empty()) { RenderFooter(enterMessage, 200); }
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