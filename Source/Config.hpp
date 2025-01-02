#pragma once
#include <string_view>
#include <array>

namespace Config
{
    // Screen settings
    inline constexpr int screenWidth = 1920;
    inline constexpr int screenHeight = 1080;

    // Assets
    constexpr std::string_view playerTexturePath = "./Assets/Ship1.png";
    constexpr std::string_view projectileTexturePath = "./Assets/Laser.png";
    constexpr std::string_view alienTexturePath = "./Assets/Alien.png";
    constexpr std::string_view wallTexturePath = "./Assets/Barrier.png";

    // Game title and messages
    inline constexpr std::string_view title = "SPACE INVADERS";
    inline constexpr std::string_view beginMessage = "PRESS SPACE TO BEGIN";
    inline constexpr std::string_view continueMessage = "PRESS ENTER TO CONTINUE";

    //Player settings
    inline constexpr float playerBaseHeight = 70.0f;
    inline constexpr float playerSpeed = 7.0f;
    inline constexpr int playerStartingLives = 3;


    // Wall settings
    inline constexpr int defaultWallCount = 5;
    inline constexpr int wallOffset = 250;

    // Alien settings
    inline constexpr int alienFormationWidth = 8;
    inline constexpr int alienFormationHeight = 5;
    inline constexpr int alienSpacing = 80;
    inline constexpr int alienFormationX = 100;
    inline constexpr int alienFormationY = 60;

    // Shooting settings
    inline constexpr float alienShootInterval = 1.0f;
    inline constexpr int projectileSpeed = 15;
    inline constexpr float defaultCooldown = 1.0f;

    // Background settings
    inline constexpr int backgroundSpeed = 15;
    inline constexpr int backgroundStarAmount = 100;
    inline constexpr int backgroundXMinOffset = -150;
    inline constexpr int backgroundXMaxOffset = 150;

    // Text positions and sizes
    inline constexpr int startScreenTitleFontSize = 160;
    inline constexpr int startScreenSubtitleFontSize = 40;
    inline constexpr int gameplayScoreFontSize = 40;
    inline constexpr int gameplayLivesFontSize = 40;
    inline constexpr int endScreenFontSize = 40;

    // UI elements
    inline constexpr int textBoxWidth = 225;
    inline constexpr int textBoxHeight = 50;
    inline constexpr int textBoxX = 600;
    inline constexpr int textBoxY = 500;

    // High scores
    inline constexpr std::array<int, 5> initialHighScores = { 500, 400, 300, 200, 100 };
}