#include "Leaderboard.hpp"
#include "Config.hpp"
#include "FileHandler.hpp"
#include "Utilities.hpp"
#include "raylib.h"
#include <gsl/gsl>
#include <algorithm>
#include <ranges>
#include <string_view>
#include <print>
#include <expected>

inline constexpr int textBoxX = 600;
inline constexpr int textBoxY = 500;
inline constexpr int textBoxWidth = 300;
inline constexpr int textBoxHeight = 50;
inline constexpr size_t leaderboardFontSize = 40;
inline constexpr int bigYOffset = 200;
inline constexpr int smallYOffset = 100;
inline constexpr int blinkTimerReset = 0;
inline constexpr std::string_view enterMessage = "PRESS ENTER TO CONTINUE";
inline constexpr std::string_view leaderboardHeader = "CURRENT LEADERBOARD";
inline constexpr std::string_view highScoreHeader = "NEW HIGHSCORE!";

struct Leaderboard::Private 
{
public:
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

    const ScoreRendering drawScoresTopLeftCorner = { 50, 100, 35, 30 };

    explicit Private(std::filesystem::path scoresPath);

    bool HasNewHighScore(int score) const noexcept;
    bool SaveHighScore(int score);
    void RenderLeaderboard() const noexcept;
    void RenderHighScoreEntry() noexcept;

private:
    void RenderScores(ScoreRendering config) const noexcept;
    void DrawScoreEntry(const PlayerData& data, const ScoreRendering& config) const noexcept;
    void InsertNewHighScore(const std::string& playerName, int score);
    [[nodiscard]] auto FindLowestScore() noexcept -> std::vector<PlayerData>::iterator;
    void ResetInputState() noexcept;
    void CapturePlayerNameInput() noexcept;
    [[nodiscard]] bool TrySaveScore(int score);
    [[nodiscard]] bool InputIsComplete() const noexcept;
    void RenderHeader(std::string_view text, int yOffset) const noexcept;
    void RenderFooter(std::string_view message, size_t yOffset) const noexcept;
    void RenderTextBox() const noexcept;
    void RenderNameInput() noexcept;
    void RenderPlayerTextInput() const noexcept;
    [[nodiscard]] bool ShouldRenderCursor() const noexcept;
    void RenderBlinkingCursor() const noexcept;
    void LoadScoresFromFile();
    std::expected<std::vector<std::pair<std::string, int>>, std::string> TryLoadScores() const;
    void PopulateScores(const std::vector<std::pair<std::string, int>>& loadedScores);
    void SaveScoresToFile();
    std::expected<void, std::string> TrySaveScores() const;
    std::vector<std::pair<std::string_view, int>> PrepareScoreViews() const;
    void HandleScoreError(std::string_view action, std::string_view error) const;

    FileHandler fileHandler;
    std::vector<PlayerData> scores;
    std::string playerName;
    Rectangle textBox;
    int blinkTimer = 0;

    static constexpr size_t maxNameLength = 9;
};

Leaderboard::Leaderboard(std::filesystem::path scores) { impl.initialize<Private>(std::move(scores)); }

Leaderboard::~Leaderboard() = default;

bool Leaderboard::HasNewHighScore(int score) const noexcept { return impl.get<Private>()->HasNewHighScore(score); }

bool Leaderboard::SaveHighScore(int score) { return impl.get<Private>()->SaveHighScore(score); }

void Leaderboard::RenderLeaderboard() const noexcept { impl.get<Private>()->RenderLeaderboard(); }

void Leaderboard::RenderHighScoreEntry() noexcept { impl.get<Private>()->RenderHighScoreEntry(); }

Leaderboard::Private::Private(std::filesystem::path scoresPath)
    : fileHandler(std::move(scoresPath)),
    textBox({ textBoxX, textBoxY, textBoxWidth, textBoxHeight })
{
    LoadScoresFromFile();
}

bool Leaderboard::Private::HasNewHighScore(int score) const noexcept
{
    return score > scores.back().score;
}

bool Leaderboard::Private::SaveHighScore(int score)
{
    CapturePlayerNameInput();
    return TrySaveScore(score);
}

void Leaderboard::Private::RenderLeaderboard() const noexcept
{
    RenderHeader(leaderboardHeader, smallYOffset);
    RenderScores(drawScoresTopLeftCorner);
    RenderFooter(enterMessage, scores.size() * (leaderboardFontSize + 10) + 50);
}

void Leaderboard::Private::RenderHighScoreEntry() noexcept
{
    RenderHeader(highScoreHeader, bigYOffset);
    RenderScores(drawScoresTopLeftCorner);
    RenderTextBox();
    RenderNameInput();
    if (!playerName.empty()) {
        RenderFooter(enterMessage, smallYOffset);
    }
}

[[nodiscard]] auto Leaderboard::Private::FindLowestScore() noexcept -> std::vector<PlayerData>::iterator
{
    return std::ranges::min_element(scores, [](const PlayerData& a, const PlayerData& b) 
        {
            return a.score < b.score;
        });
}

void Leaderboard::Private::InsertNewHighScore(const std::string& name, int score)
{
    auto lowestScore = FindLowestScore();
    if (lowestScore != scores.end() && score > lowestScore->score)
    {
        *lowestScore = { name, score };
    }

    std::ranges::sort(scores, [](const PlayerData& a, const PlayerData& b) 
        {
            return a.score > b.score;
        });
    SaveScoresToFile();
}

void Leaderboard::Private::ResetInputState() noexcept
{
    playerName.clear();
    blinkTimer = blinkTimerReset;
}

void Leaderboard::Private::CapturePlayerNameInput() noexcept
{
    const int key = GetCharPressed();
    if ((key >= 32) && (key <= 125) && playerName.size() < maxNameLength)
    {
        playerName.push_back(gsl::narrow_cast<char>(key));
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) { playerName.pop_back(); }
    blinkTimer++;
}

[[nodiscard]] bool Leaderboard::Private::TrySaveScore(int score)
{
    if (!InputIsComplete()) { return false; }
    InsertNewHighScore(playerName, score);
    ResetInputState();
    return true;
}

[[nodiscard]] bool Leaderboard::Private::InputIsComplete() const noexcept
{
    return !playerName.empty() && IsKeyReleased(KEY_ENTER);
}

void Leaderboard::Private::RenderScores(ScoreRendering screenPos) const noexcept
{
    std::ranges::for_each(scores, [&](const PlayerData& data) noexcept {
        DrawScoreEntry(data, screenPos);
        screenPos.currentY += screenPos.ySpacing;
        });
}

void Leaderboard::Private::DrawScoreEntry(const PlayerData& data, const ScoreRendering& config) const noexcept
{
    DrawText(data.name.c_str(), config.startX, config.currentY, config.fontSize, YELLOW);
    DrawText(TextFormat("%d", data.score), config.startX + 300, config.currentY, config.fontSize, YELLOW);
}

void Leaderboard::Private::RenderHeader(std::string_view text, int yOffset) const noexcept
{
    DrawText(text.data(), textBoxX, textBoxY + yOffset, leaderboardFontSize, YELLOW);
}

void Leaderboard::Private::RenderFooter(std::string_view message, size_t yOffset) const noexcept
{
    DrawText(message.data(), textBoxX, textBoxY + gsl::narrow_cast<int>(yOffset), leaderboardFontSize, YELLOW);
}

void Leaderboard::Private::RenderTextBox() const noexcept
{
    DrawRectangleRec(textBox, LIGHTGRAY);
    Util::DrawRectangleLines(textBox, RED);
}

void Leaderboard::Private::RenderNameInput() noexcept
{
    RenderPlayerTextInput();
    if (ShouldRenderCursor()) { RenderBlinkingCursor(); }
}

void Leaderboard::Private::RenderPlayerTextInput() const noexcept
{
    DrawText(playerName.c_str(), static_cast<int>(textBox.x) + 5,
        static_cast<int>(textBox.y) + 8, leaderboardFontSize, MAROON);
}

[[nodiscard]] bool Leaderboard::Private::ShouldRenderCursor() const noexcept
{
    return playerName.size() < maxNameLength && ((blinkTimer / 20) % 2) == 0;
}

void Leaderboard::Private::RenderBlinkingCursor() const noexcept
{
    DrawText("_", static_cast<int>(textBox.x) + 8 + MeasureText(playerName.c_str(), leaderboardFontSize),
        static_cast<int>(textBox.y) + 12, leaderboardFontSize, MAROON);
}

void Leaderboard::Private::LoadScoresFromFile()
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
Leaderboard::Private::TryLoadScores() const
{
    return fileHandler.LoadScores();
}

void Leaderboard::Private::PopulateScores(const std::vector<std::pair<std::string, int>>& loadedScores)
{
    scores.clear();
    std::ranges::transform(loadedScores, std::back_inserter(scores), [](const auto& pair) 
        {
            return PlayerData{ pair.first, pair.second };
        });
}

void Leaderboard::Private::SaveScoresToFile()
{
    auto saveResult = TrySaveScores();
    if (!saveResult) { HandleScoreError("saving", saveResult.error()); }
}

[[nodiscard]] std::expected<void, std::string> Leaderboard::Private::TrySaveScores() const
{
    auto scoreViews = PrepareScoreViews();
    return fileHandler.SaveScores(scoreViews);
}

[[nodiscard]] std::vector<std::pair<std::string_view, int>> Leaderboard::Private::PrepareScoreViews() const
{
    std::vector<std::pair<std::string_view, int>> scoreViews;
    scoreViews.reserve(scores.size());
    std::ranges::transform(scores, std::back_inserter(scoreViews), [](const PlayerData& data) {
        return std::make_pair(std::string_view(data.name), data.score);
        });
    return scoreViews;
}

void Leaderboard::Private::HandleScoreError(std::string_view action, std::string_view error) const
{
    std::print("Error {} scores: {}\n", action, error);
}