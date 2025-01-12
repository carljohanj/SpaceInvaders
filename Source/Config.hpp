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
    inline constexpr std::string_view beginMessage    = "PRESS SPACE TO BEGIN";
    inline constexpr std::string_view continueMessage = "PRESS ENTER TO CONTINUE";

    // Assets
    inline const std::filesystem::path playerTexturePath     = "./Resources/Assets/Ship1.png";
    inline const std::filesystem::path projectileTexturePath = "./Resources/Assets/Laser.png";
    inline const std::filesystem::path alienTexturePath      = "./Resources/Assets/Alien.png";
    inline const std::filesystem::path wallTexturePath       = "./Resources/Assets/Barrier.png";
    inline const std::filesystem::path leaderBoardScores     = "./Resources/leaderboard.txt";

    // Sound effects
    inline const std::filesystem::path backgroundMusic = "./Resources/Sounds/chip-mode.mp3";
    inline const std::filesystem::path startGame       = "./Resources/Sounds/StartSound.wav";
    inline const std::filesystem::path playerShot      = "./Resources/Sounds/playerShooting.wav";
    inline const std::filesystem::path alienShot       = "./Resources/Sounds/alienShooting.wav";
    inline const std::filesystem::path highScore       = "./Resources/Sounds/newHighScore2.wav";
    inline const std::filesystem::path dead            = "./Resources/Sounds/death.wav";
    inline const std::filesystem::path hitSound        = "./Resources/Sounds/hit.wav";

    // Texture scaling sizes
    inline constexpr int alienWidth  = 100;
    inline constexpr int alienHeight = 100;

    inline constexpr int wallWidth  = 200;
    inline constexpr int wallHeight = 200;

    inline constexpr int playerWidth  = 100;
    inline constexpr int playerHeight = 100;

    inline constexpr int projectileWidth  = 50;
    inline constexpr int projectileHeight = 50;

    //Player settings
    inline constexpr float playerBaseHeight  = 70.0f;
    inline constexpr float playerSpeed       = 7.0f;
    inline constexpr int playerStartingLives = 3;

    // Wall settings
    inline constexpr int defaultWallCount = 5;
    inline constexpr int wallOffset       = 250;

    // Alien settings
    inline constexpr int alienFormationWidth  = 8;
    inline constexpr int alienFormationHeight = 5;
    inline constexpr int alienSpacing         = 80;
    inline constexpr int alienFormationX      = 100;
    inline constexpr int alienFormationY      = 60;

    // Shooting settings
    inline constexpr float alienShootInterval  = 1.5f;
    inline constexpr float alienGunCooldown    = 1.0f;
    inline constexpr int playerProjectileSpeed = -15;
    inline constexpr int alienProjectileSpeed  = 15;

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
    inline constexpr int textBoxX                    = 600;
    inline constexpr int textBoxY                    = 500;
}