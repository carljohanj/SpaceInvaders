#include "Leaderboard.hpp"
#include "Config.hpp"
#include <gsl/gsl>
#include "raylib.h"
#include "Utilities.hpp"
#include <algorithm>
#include <ranges>
#include <string_view>
#include <print>

inline constexpr int textBoxX = 600;
inline constexpr int textBoxY = 500;
inline constexpr int textBoxWidth = 300;
inline constexpr int textBoxHeight = 50;
inline constexpr size_t leaderboardFontSize = 40;
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

Leaderboard::Leaderboard(std::filesystem::path scores)
    : fileHandler(scores),
    textBox({ textBoxX, textBoxY, textBoxWidth, textBoxHeight })
{
    LoadScoresFromFile();
}

[[nodiscard]] bool Leaderboard::HasNewHighScore(int score) const noexcept { return score > scores.back().score; }

[[nodiscard]] bool Leaderboard::SaveHighScore(int score)
{
    CapturePlayerNameInput();
    return TrySaveScore(score);
}

void Leaderboard::CapturePlayerNameInput() noexcept
{
    const int key = GetCharPressed();
    if ((key >= 32) && (key <= 125) && playerName.size() < maxNameLength)
    {
        playerName.push_back(gsl::narrow_cast<char>(key));
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) { playerName.pop_back(); }
    blinkTimer++;
}

[[nodiscard]] inline bool Leaderboard::TrySaveScore(int score)
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

inline void Leaderboard::RenderFooter(std::string_view message, size_t yOffset) const noexcept
{
    DrawText(message.data(), textBoxX, textBoxY + gsl::narrow_cast<int>(yOffset), leaderboardFontSize, YELLOW);
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
    Util::DrawRectangleLines(textBox, RED);
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
    auto fileContent = TryLoadScores();
    if (fileContent)
    {
        PopulateScores(fileContent.value());
        return;
    }
    HandleScoreError("loading", fileContent.error());
    scores.clear();
}

[[nodiscard]] std::expected<std::vector<std::pair<std::string, int>>, std::string>
Leaderboard::TryLoadScores() const
{
    return fileHandler.LoadScores();
}

void Leaderboard::PopulateScores(const std::vector<std::pair<std::string, int>>& loadedScores)
{
    scores.clear();
    std::ranges::transform(loadedScores, std::back_inserter(scores), [](const auto& pair)
        {
            return PlayerData{ pair.first, pair.second };
        });
}

void Leaderboard::SaveScoresToFile()
{
    auto const saveResult = TrySaveScores();
    if (!saveResult)
    {
        HandleScoreError("saving", saveResult.error());
    }
}

[[nodiscard]] std::expected<void, std::string> Leaderboard::TrySaveScores() const
{
    auto const scoreViews = PrepareScoreViews();
    return fileHandler.SaveScores(scoreViews);
}

[[nodiscard]] std::vector<std::pair<std::string_view, int>> Leaderboard::PrepareScoreViews() const
{
    std::vector<std::pair<std::string_view, int>> scoreViews;
    scoreViews.reserve(scores.size());
    std::ranges::transform(scores, std::back_inserter(scoreViews), [](const PlayerData& data)
        {
            return std::make_pair(std::string_view(data.name), data.score);
        });
    return scoreViews;
}

void Leaderboard::HandleScoreError(std::string_view action, std::string_view error) const
{
    std::print("Error {} scores: {}\n", action, error);
}