#pragma once
#include "StackManager.hpp"
#include <filesystem>

class Leaderboard
{
public:
    explicit Leaderboard(std::filesystem::path scores);

    [[nodiscard]] bool HasNewHighScore(int score) const noexcept;
    [[nodiscard]] bool SaveHighScore(int score);
    void RenderLeaderboard() const noexcept;
    void RenderHighScoreEntry() noexcept;

private:
    struct Private;
    class StackManager<200> impl;
};