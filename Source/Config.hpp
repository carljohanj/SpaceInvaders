#pragma once
#include <filesystem>
#include <string_view>
#include <array>

namespace Config
{
    // Window settings
    inline constexpr int screenWidth  = 1920;
    inline constexpr int screenHeight = 1080;
    inline constexpr std::string_view title = "SPACE INVADERS";

    // Menus and messages
    inline constexpr std::string_view beginMessage = "PRESS SPACE TO BEGIN";
    inline constexpr std::string_view continueMessage = "PRESS ENTER TO CONTINUE";

    // Assets
    inline const std::filesystem::path playerTexturePath     = "./Assets/Ship1.png";
    inline const std::filesystem::path projectileTexturePath = "./Assets/Laser.png";
    inline const std::filesystem::path alienTexturePath      = "./Assets/Alien.png";
    inline const std::filesystem::path wallTexturePath       = "./Assets/Barrier.png";

    //Player settings
    inline constexpr float playerBaseHeight   = 70.0f;
    inline constexpr float playerSpeed        = 7.0f;
    inline constexpr int playerStartingLives  = 3;

    // Wall settings
    inline constexpr int defaultWallCount     = 5;
    inline constexpr int wallOffset           = 250;

    // Alien settings
    inline constexpr int alienFormationWidth  = 8;
    inline constexpr int alienFormationHeight = 5;
    inline constexpr int alienSpacing         = 80;
    inline constexpr int alienFormationX      = 100;
    inline constexpr int alienFormationY      = 60;

    // Shooting settings
    inline constexpr float alienShootInterval = 1.0f;
    inline constexpr float alienGunCooldown   = 1.0f;
    inline constexpr int projectileSpeed      = 15;

    // Background settings
    inline constexpr int backgroundSpeed      = 15;
    inline constexpr int backgroundStarAmount = 100;
    inline constexpr int backgroundXMinOffset = -150;
    inline constexpr int backgroundXMaxOffset = 150;

    // Text sizes
    inline constexpr int startScreenTitleFontSize    = 160;
    inline constexpr int startScreenSubtitleFontSize = 40;
    inline constexpr int gameplayScoreFontSize       = 40;
    inline constexpr int gameplayLivesFontSize       = 40;
    inline constexpr int endScreenFontSize           = 40;

    // High scores
    inline constexpr std::array<int, 5> initialHighScores = { 500, 400, 300, 200, 100 };
}