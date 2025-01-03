#include "Leaderboard.hpp"
#include "Config.hpp"
#include "raylib.h"
#include <algorithm>
#include <ranges>
#include <string_view>

// Constants
inline constexpr int textBoxX = 600;
inline constexpr int textBoxY = 500;
inline constexpr int textBoxWidth = 300;
inline constexpr int textBoxHeight = 50;
inline constexpr int leaderboardFontSize = 40;
inline constexpr int highScoreFontSize = 60;
inline constexpr int maxNameLength = 9;
inline constexpr std::string_view enterMessage = "PRESS ENTER OR SPACE TO CONTINUE";
inline constexpr std::string_view leaderboardHeader = "LEADERBOARD";
inline constexpr std::string_view highScoreHeader = "NEW HIGHSCORE!";

// Constructor
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
    HandleTextInput();
    return TrySaveScore(score);
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
    std::ranges::sort(scores, [](const PlayerData& a, const PlayerData& b) { return a.score > b.score; });
}

void Leaderboard::RenderLeaderboard() const noexcept
{
    RenderHeader(leaderboardHeader, -100);
    RenderScores();
    RenderFooter(enterMessage, scores.size() * (leaderboardFontSize + 10) + 50);
}

inline void Leaderboard::RenderScores() const noexcept
{
    std::ranges::for_each (scores | std::views::enumerate, [&] (const auto& entry) noexcept
        {
            DrawScoreEntry(entry);
        });
}

inline void Leaderboard::DrawScoreEntry(const std::pair<size_t, PlayerData>& entry) const noexcept
{
    const auto& [index, data] = entry;
    const int yOffset = textBoxY + static_cast<int>(index) * (leaderboardFontSize + 10);
    DrawText(data.name.c_str(), textBoxX, yOffset, leaderboardFontSize, YELLOW);
    DrawText(TextFormat("%d", data.score), textBoxX + 300, yOffset, leaderboardFontSize, YELLOW);
}

inline void Leaderboard::RenderHeader(std::string_view text, int yOffset) const noexcept
{
    DrawText(text.data(), textBoxX, textBoxY + yOffset, leaderboardFontSize, YELLOW);
}

inline void Leaderboard::RenderFooter(std::string_view message, int yOffset) const noexcept
{
    DrawText(message.data(), textBoxX, textBoxY + yOffset, leaderboardFontSize, YELLOW);
}







void Leaderboard::RenderHighScoreEntry(int score) const noexcept
{
    RenderHeader(highScoreHeader, -200);
    std::ranges::for_each(scores | std::views::enumerate, [&](auto entry) noexcept {
        const auto& [index, data] = entry;
        DrawText(data.name.c_str(), 50, 100 + index * (leaderboardFontSize - 10), leaderboardFontSize - 10, YELLOW);
        DrawText(TextFormat("%d", data.score), 250, 100 + index * (leaderboardFontSize - 10), leaderboardFontSize - 10, YELLOW);
        });

    DrawRectangleRec(textBox, LIGHTGRAY);
    DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);

    DrawText(playerName.c_str(), (int)textBox.x + 5, (int)textBox.y + 8, leaderboardFontSize, MAROON);

    if (playerName.size() < maxNameLength && ((framesCounter / 20) % 2) == 0) {
        DrawText("_", (int)textBox.x + 8 + MeasureText(playerName.c_str(), leaderboardFontSize), (int)textBox.y + 12, leaderboardFontSize, MAROON);
    }

    if (!playerName.empty()) { RenderFooter("PRESS ENTER TO CONTINUE", 200); }
}

void Leaderboard::HandleTextInput() noexcept 
{
    SetMouseCursor(MOUSE_CURSOR_IBEAM);

    int key = GetCharPressed();
    while (key > 0) 
    {
        if ((key >= 32) && (key <= 125) && playerName.size() < maxNameLength) 
        {
            playerName.push_back(static_cast<char>(key));
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) { playerName.pop_back(); }
    framesCounter++;
}

void Leaderboard::ResetInputState() noexcept 
{
    playerName.clear();
    framesCounter = 0;
}