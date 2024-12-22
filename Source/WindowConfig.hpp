#pragma once
#include <string>

// Screen dimensions
constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;

// Game title
const char* title = "SPACE INVADERS";

// Walls
constexpr int defaultWallCount = 5;

// Alien formation
constexpr int alienFormationWidth = 8;
constexpr int alienFormationHeight = 5;
constexpr int alienSpacing = 80;
constexpr int alienFormationX = 100;
constexpr int alienFormationY = 50;

// Shooting
constexpr float alienShootInterval = 1.0f;

// Background
constexpr int backgroundSpeed = 15;

// Text positions and sizes
constexpr int startScreenTitleFontSize = 160;
constexpr int startScreenSubtitleFontSize = 40;
constexpr int gameplayScoreFontSize = 40;
constexpr int gameplayLivesFontSize = 40;
constexpr int endScreenFontSize = 40;

// Other UI
constexpr int textBoxWidth = 225;
constexpr int textBoxHeight = 50;
constexpr int textBoxX = 600;
constexpr int textBoxY = 500;

// High Scores
constexpr int initialHighScores[5] = { 500, 400, 300, 200, 100 };