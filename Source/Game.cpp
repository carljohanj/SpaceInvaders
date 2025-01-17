#pragma once
#include "Alien.hpp"
#include "AudioDevice.hpp"
#include "Background.hpp"
#include "Config.hpp"
#include <filesystem>
#include "Game.hpp"
#include "GameWindow.hpp"
#include "Leaderboard.hpp"
#include "Player.hpp"
#include "raylib.h"
#include "StackManager.hpp"
#include "SoundEffects.hpp"
#include "Utilities.hpp"
#include "Wall.hpp"
#include <algorithm>
#include <cassert>
#include <gsl/gsl>
#include <random>
#include <ranges>

inline constexpr float shootTimerReset = 0.0f;
inline constexpr int addToScore = 100;
inline constexpr int alienFormationOffset = 450;

struct Game::Private
{
	Private(int screenWidth, int screenHeight)
		: window(screenWidth, screenHeight),
		  leaderboard(Config::leaderBoardScores) {
	}

	enum class State { STARTSCREEN, GAMEPLAY, ENDSCREEN, LEADERBOARD };

	struct PlayerData
	{
		std::string name;
		int score;
	};

	State gameState = State::STARTSCREEN;
	int score = 0;
	float shootTimer = Config::alienGunCooldown;
	bool newHighScore = false;

	GameWindow window;
	Player player;
	std::vector<Alien> Aliens;
	std::vector<Projectile> Projectiles;
	std::vector<Wall> Walls;
	Leaderboard leaderboard;
	AudioDevice audioDevice;
	Background background;

	void Start();
	void Update();
	void Render();
	void RenderState();
	void End() noexcept;
	void RenderEndScreen() noexcept;
	void UpdateStartScreen() noexcept;
	void Continue();
	void ResetGameState() noexcept;
	void ResetAliens();
	void RenderGameplay();
	void UpdateGameplay();
	void UpdatePlayerInput();
	void CreateNewAlien(int alienRow);
	void RenderAliens();
	void UpdateAliens();
	void UpdateAlienstates() noexcept;
	void TriggerAlienShot() noexcept;
	void UpdateTimeSinceLastShot() noexcept;
	bool CanFireShot() const noexcept;
	void FireShot() noexcept;
	void ResetShootTimer() noexcept;
	void RemoveInactiveAliens() noexcept;
	void ResetWalls();
	void RenderWalls();
	void UpdateWalls() noexcept;
	void RenderProjectiles() const noexcept;
	void UpdateProjectiles();
	void UpdateBackground(float playerPos) noexcept;
	void UpdateEndScreen();
	void UpdateShowLeaderboard();
	void RenderShowLeaderboard() const noexcept;
	void DetectCollisions() noexcept;
	void CheckPlayerCollision(Projectile& projectile) noexcept;
	void AlienGetsShot(Alien& alien, Projectile& projectile) noexcept;
	void WallGetsHit(Wall& wall, Projectile& projectile) noexcept;
	void PlayerGetsShot(Projectile& projectile) noexcept;
	void CheckEnemyCollision(Projectile& projectile) noexcept;
	void CheckWallCollisions(Projectile& projectile) noexcept;
};

Game::Game(int screenWidth, int screenHeight)
{
	impl.initialize<Private>(screenWidth, screenHeight);
}

void Game::Run()
{
	auto* game = impl.get<Private>();
	assert(game != nullptr);

	game->Start();
	while (!WindowShouldClose())
	{
		game->Update();
		game->Render();
	}
}

void Game::Private::Start()
{
	ResetWalls();
	player.Reset();
	ResetAliens();
	background.Reset();
	ResetGameState();
}

void Game::Private::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		UpdateStartScreen();
		break;
	case State::GAMEPLAY:
		UpdateGameplay();
		break;
	case State::ENDSCREEN:
		UpdateEndScreen();
		break;
	case State::LEADERBOARD:
		UpdateShowLeaderboard();
		break;
	}
}

void Game::Private::Render()
{
	window.BeginDrawing();
	ClearBackground(BLACK);
	RenderState();
	window.EndDrawing();
}


void Game::Private::RenderState()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		window.RenderStartScreen();
		break;
	case State::GAMEPLAY:
		RenderGameplay();
		break;
	case State::ENDSCREEN:
		RenderEndScreen();
		break;
	case State::LEADERBOARD:
		RenderShowLeaderboard();
		break;
	}
}

void Game::Private::End() noexcept
{
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	gameState = State::ENDSCREEN;
	audioDevice.Play(SoundEffect::PlayerDies);
	audioDevice.StopBackgroundMusic();
}

void Game::Private::RenderEndScreen() noexcept
{
	if (leaderboard.HasNewHighScore(score)) { leaderboard.RenderHighScoreEntry(); }
	else { leaderboard.RenderLeaderboard(); }
}

inline void Game::Private::UpdateStartScreen() noexcept
{
	if (IsKeyReleased(KEY_SPACE))
	{
		ResetGameState();
		gameState = State::GAMEPLAY;
		audioDevice.Play(SoundEffect::StartSound);
		audioDevice.PlayBackgroundMusic();
	}
}

inline void Game::Private::UpdateEndScreen()
{
	if (leaderboard.HasNewHighScore(score) && leaderboard.SaveHighScore(score))
	{
		gameState = State::LEADERBOARD;
		audioDevice.Play(SoundEffect::NewHighScore);
		return;
	}
	if (IsKeyReleased(KEY_ENTER)) { Continue(); }
}

inline void Game::Private::UpdateShowLeaderboard()
{
	if (IsKeyReleased(KEY_SPACE) || IsKeyReleased(KEY_ENTER)) { Continue(); }
}

void Game::Private::RenderShowLeaderboard() const noexcept { leaderboard.RenderLeaderboard(); }

inline void Game::Private::Continue()
{
	Start();
	gameState = State::STARTSCREEN;
}

inline void Game::Private::ResetGameState() noexcept
{
	score = 0;
	gameState = State::STARTSCREEN;
}

void Game::Private::RenderGameplay()
{
	background.Render();
	window.RenderHUD(score, player.GetLives());
	player.Render();
	RenderProjectiles();
	RenderWalls();
	RenderAliens();
}

void Game::Private::UpdateGameplay()
{
	UpdatePlayerInput();
	UpdateAliens();
	UpdateProjectiles();
	DetectCollisions();
	UpdateWalls();
	UpdateBackground(player.GetXPosition());
	audioDevice.UpdateMusic();
}

void Game::Private::UpdatePlayerInput()
{
	if (IsKeyReleased(KEY_Q)) { End(); }
	player.Update();
	if (IsKeyPressed(KEY_SPACE))
	{
		Projectiles.push_back(player.Shoot());
		audioDevice.Play(SoundEffect::PlayerShoots);
	}
}

void Game::Private::ResetAliens()
{
	if (!Aliens.empty()) return;
	for (const int alienRow : std::views::iota(0, Config::alienFormationHeight))
	{
		CreateNewAlien(alienRow);
	}
}

inline void Game::Private::CreateNewAlien(int alienRow)
{
	for (const int alienCol : std::views::iota(0, Config::alienFormationWidth))
	{
		Aliens.emplace_back( Config::alienFormationX + alienFormationOffset + (alienCol * Config::alienSpacing),
			Config::alienFormationY + (alienRow * Config::alienSpacing)
		);
	}
}

void Game::Private::RenderAliens()
{
	for (const auto& alien : Aliens)
	{
		alien.Render();
	}
}

void Game::Private::UpdateAliens()
{
	RemoveInactiveAliens();
	TriggerAlienShot();
	UpdateAlienstates();
	if (Aliens.empty()) { ResetAliens(); }
}

inline void Game::Private::UpdateAlienstates() noexcept
{
	for (auto& alien : Aliens)
	{
		alien.Update();
		if (alien.GetPosition().y > Config::screenHeight - Config::playerBaseHeight) { End(); }
	}
}

void Game::Private::TriggerAlienShot() noexcept
{
	UpdateTimeSinceLastShot();
	if (CanFireShot())
	{
		FireShot();
		ResetShootTimer();
	}
}

inline void Game::Private::UpdateTimeSinceLastShot() noexcept { shootTimer += GetFrameTime(); }

inline bool Game::Private::CanFireShot() const noexcept
{
	return Aliens.empty() ? false : shootTimer >= Config::alienShootInterval;
}

inline void Game::Private::FireShot() noexcept
{
	auto const randomIndexValue = GetRandomValue(0, gsl::narrow_cast<int>(Aliens.size()) - 1);
	auto const& randomAlien = Aliens[randomIndexValue];
	if (randomAlien.IsActive())
	{
		Projectiles.push_back(randomAlien.Shoot());
		audioDevice.Play(SoundEffect::AlienShoots);
	}
}

inline void Game::Private::ResetShootTimer() noexcept { shootTimer = shootTimerReset; }

inline void Game::Private::RemoveInactiveAliens() noexcept
{
	std::erase_if(Aliens, [](const Alien& alien) noexcept { return !alien.IsActive(); });
}

void Game::Private::ResetWalls()
{
	Walls.clear();
	constexpr auto wallDistance = Config::screenWidth / (Config::defaultWallCount + 1);
	for (const int i : std::views::iota(1, Config::defaultWallCount + 1))
	{
		Walls.emplace_back().SetPosition({ wallDistance * static_cast<float>(i),
										   Config::screenHeight - Config::wallOffset });
	}
}

void Game::Private::RenderWalls()
{
	for (const auto& wall : Walls)
	{
		wall.Render();
	}
}

void Game::Private::UpdateWalls() noexcept
{
	std::erase_if(Walls, [](const Wall& wall) noexcept { return !wall.IsActive(); });
}

void Game::Private::RenderProjectiles() const noexcept
{
	for (const auto& projectile : Projectiles)
	{
		projectile.Render();
	}
}

void Game::Private::UpdateProjectiles()
{
	for (auto& projectile : Projectiles)
	{
		projectile.Update();
	}
	std::erase_if(Projectiles, [](const Projectile& p) noexcept { return !p.IsActive(); });
}

inline void Game::Private::UpdateBackground(float playerPos) noexcept
{
	const Vector2 playerPosition = { playerPos, Config::playerBaseHeight };
	const Vector2 screenCorner = { 0.0f, Config::playerBaseHeight };
	const auto dx = screenCorner.x - playerPosition.x;
	const auto dy = screenCorner.y - playerPosition.y;
	const auto backgroundOffset = std::sqrt(dx * dx + dy * dy) * -1.0f;
	background.Update(backgroundOffset / Config::backgroundSpeed);
}

void Game::Private::DetectCollisions() noexcept
{
	for (auto& projectile : Projectiles)
	{
		if (!projectile.IsActive()) { continue; }
		switch (projectile.GetType())
		{
		case ProjectileType::PLAYER_PROJECTILE:
			CheckEnemyCollision(projectile);
			break;
		case ProjectileType::ENEMY_PROJECTILE:
			CheckPlayerCollision(projectile);
			break;
		}
		CheckWallCollisions(projectile);
	}
}

inline void Game::Private::CheckEnemyCollision(Projectile& projectile) noexcept
{
	for (auto& alien : Aliens)
	{
		if (!alien.IsActive()) { continue; }
		if (CheckCollisionCircles(alien.GetPosition(), alien.GetRadius(),
			{ projectile.GetLineStart().x, projectile.GetLineStart().y }, 1.0f))
		{
			AlienGetsShot(alien, projectile);
			break;
		}
	}
}

inline void Game::Private::AlienGetsShot(Alien& alien, Projectile& projectile) noexcept
{
	alien.SetActive(false);
	projectile.SetActive(false);
	audioDevice.Play(SoundEffect::IsHit);
	score += addToScore;
}

inline void Game::Private::CheckPlayerCollision(Projectile& projectile) noexcept
{
	if (CheckCollisionCircles(player.GetPosition(), player.GetRadius(),
		projectile.GetPositionForCollision(), 1.0f)) { PlayerGetsShot(projectile); }
}

inline void Game::Private::PlayerGetsShot(Projectile& projectile) noexcept
{
	player.SetLives(player.GetLives() - 1);
	projectile.SetActive(false);
	audioDevice.Play(SoundEffect::IsHit);
	if (player.GetLives() <= 0) { End(); }
}

inline void Game::Private::CheckWallCollisions(Projectile& projectile) noexcept
{
	for (auto& wall : Walls)
	{
		if (!wall.IsActive()) { continue; }
		if (CheckCollisionPointRec(projectile.GetPosition(), wall.GetRectangle()))
		{
			WallGetsHit(wall, projectile);
			break;
		}
	}
}

inline void Game::Private::WallGetsHit(Wall& wall, Projectile& projectile) noexcept
{
	wall.SetHealth(wall.GetHealth() - 1);
	if (wall.GetHealth() <= 0) { wall.SetActive(false); }
	audioDevice.Play(SoundEffect::IsHit);
	projectile.SetActive(false);
}