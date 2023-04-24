/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include "game_level.h"
#include "power_up.h"

// clang-format off
#include <glad/glad.h>  // GLAD must be included before GLFW
#include <GLFW/glfw3.h>
// clang-format on

#include <tuple>
#include <vector>

// Represents the current state of the game
enum GameState
{
  GAME_ACTIVE,
  GAME_MENU,
  GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction
{
  UP,
  RIGHT,
  DOWN,
  LEFT
};

namespace Classic
{
constexpr float accelerationFactor = 1.0f;
}

namespace HardMode
{
constexpr float accelerationFactor = 1.05f;
}

// Defines a Collision typedef that represents collision data
typedef std::tuple<bool, Direction, glm::vec2>
    Collision;  // <collision?, what direction?, difference vector center -
                // closest point>

// TODO: Change to lowercase
// TODO: Use constexpr/constinit
// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
  // constructor/destructor
  Game(unsigned int width, unsigned int height);
  ~Game();
  // initialize game state (load all shaders/textures/levels)
  void Init();
  // game loop
  void ProcessInput(float dt);
  void Update(float dt);
  void Render();

  // Public data
  // TODO: Make private. These need to be public for the key_callback function.
  bool Keys[1024];
  bool KeysProcessed[1024];

private:
  // Collisions
  void DoCollisions();

  // Reset
  void ResetLevel();
  void ResetPlayer();

  // Power ups
  void SpawnPowerUps(GameObject& block);
  void UpdatePowerUps(float dt);

  // Hard mode
  void ToggleHardMode();

  // Data
  GameState State;
  unsigned int Width, Height;
  std::vector<GameLevel> Levels;
  std::vector<PowerUp> PowerUps;
  unsigned int Level;
  unsigned int Lives;

  struct Options
  {
    bool hardModeOn = false;
    float accelerationFactor = Classic::accelerationFactor;
  };
  Options m_options {};
};

#endif
