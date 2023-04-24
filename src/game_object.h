/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "sprite_renderer.h"
#include "texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
struct GameObject
{
  // constructor(s)
  GameObject() = default;
  // TODO: Consider reorder parameters to match member variable order.
  GameObject(glm::vec2 pos,
             glm::vec2 size,
             Texture2D sprite,
             glm::vec3 color = glm::vec3(1.0f),
             glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
  // draw sprite
  virtual void Draw(SpriteRenderer& renderer);

  // Data
  // Object state
  glm::vec2 Position {0.0f, 0.0f};
  glm::vec2 Size {1.0f, 1.0f};
  glm::vec2 Velocity {0.0f};
  // TODO: Add constants for colors
  glm::vec3 Color {1.0f};
  float Rotation {0.0f};

  bool IsSolid = false;
  bool Destroyed = false;

  // Render state
  Texture2D Sprite {};
};

#endif
