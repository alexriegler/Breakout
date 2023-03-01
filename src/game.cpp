/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ball_object.h"
#include "game.h"
#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

// Game-related State data
// TODO: Use smart pointers
SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE)
    , Keys()
    , Width(width)
    , Height(height)
{
}

Game::~Game()
{
  delete Renderer;
  delete Player;
}

void Game::Init()
{
  // load shaders
  ResourceManager::LoadShader(
      "shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
  // configure shaders
  glm::mat4 projection = glm::ortho(0.0f,
                                    static_cast<float>(this->Width),
                                    static_cast<float>(this->Height),
                                    0.0f,
                                    -1.0f,
                                    1.0f);
  ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
  ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
  // set render-specific controls
  Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
  // load textures
  // TODO: Set a default directory for textures, etc.
  // TODO: Default name of a texture should the file's name without the
  //       extension
  ResourceManager::LoadTexture("textures/background.jpg", false, "background");
  ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
  ResourceManager::LoadTexture("textures/block.png", false, "block");
  ResourceManager::LoadTexture(
      "textures/block_solid.png", false, "block_solid");
  ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
  // load levels
  // TODO: Improve level loading
  GameLevel one;
  one.Load("levels/one.lvl", this->Width, this->Height / 2);
  GameLevel two;
  two.Load("levels/two.lvl", this->Width, this->Height / 2);
  GameLevel three;
  three.Load("levels/three.lvl", this->Width, this->Height / 2);
  GameLevel four;
  four.Load("levels/four.lvl", this->Width, this->Height / 2);
  this->Levels.push_back(one);
  this->Levels.push_back(two);
  this->Levels.push_back(three);
  this->Levels.push_back(four);
  this->Level = 0;
  // configure game objects
  glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                  this->Height - PLAYER_SIZE.y);
  Player = new GameObject(
      playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

  glm::vec2 ballPos = playerPos
      + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
  Ball = new BallObject(ballPos,
                        BALL_RADIUS,
                        INITIAL_BALL_VELOCITY,
                        ResourceManager::GetTexture("face"));
}

void Game::Update(float dt)
{
  // update objects
  Ball->Move(dt, this->Width);
  // check for collisions
  this->DoCollisions();
}

void Game::ProcessInput(float dt)
{
  if (this->State == GAME_ACTIVE) {
    float velocity = PLAYER_VELOCITY * dt;
    // move playerboard
    if (this->Keys[GLFW_KEY_A]) {
      if (Player->Position.x >= 0.0f) {
        Player->Position.x -= velocity;
        if (Ball->Stuck)
          Ball->Position.x -= velocity;
      }
    }
    if (this->Keys[GLFW_KEY_D]) {
      if (Player->Position.x <= this->Width - Player->Size.x) {
        Player->Position.x += velocity;
        if (Ball->Stuck)
          Ball->Position.x += velocity;
      }
    }
    if (this->Keys[GLFW_KEY_SPACE])
      Ball->Stuck = false;
  }
}

void Game::Render()
{
  if (this->State == GAME_ACTIVE) {
    // draw background
    Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                         glm::vec2(0.0f, 0.0f),
                         glm::vec2(this->Width, this->Height),
                         0.0f);
    // draw level
    this->Levels[this->Level].Draw(*Renderer);
    // draw player
    Player->Draw(*Renderer);
    // draw ball
    Ball->Draw(*Renderer);
  }
}

bool CheckCollision(GameObject& one, GameObject& two)  // AABB - AABB collision
{
  // collision x-axis?
  bool collisionX = one.Position.x + one.Size.x >= two.Position.x
      && two.Position.x + two.Size.x >= one.Position.x;
  // collision y-axis?
  bool collisionY = one.Position.y + one.Size.y >= two.Position.y
      && two.Position.y + two.Size.y >= one.Position.y;
  // collision only if on both axes
  return collisionX && collisionY;
}

bool CheckCollision(BallObject& one,
                    GameObject& two)  // AABB - Circle collision
{
  // get center point circle first
  glm::vec2 center(one.Position + one.Radius);
  // calculate AABB info (center, half-extents)
  glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
  glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x,
                        two.Position.y + aabb_half_extents.y);
  // get difference vector between both centers
  glm::vec2 difference = center - aabb_center;
  glm::vec2 clamped =
      glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
  // add clamped value to AABB_center and we get the value of box closest to
  // circle
  glm::vec2 closest = aabb_center + clamped;
  // retrieve vector between center circle and closest point AABB and check if
  // length <= radius
  difference = closest - center;
  return glm::length(difference) < one.Radius;
}

void Game::DoCollisions()
{
  for (GameObject& box : this->Levels[this->Level].Bricks) {
    if (!box.Destroyed) {
      if (CheckCollision(*Ball, box)) {
        if (!box.IsSolid)
          box.Destroyed = true;
      }
    }
  }
}
