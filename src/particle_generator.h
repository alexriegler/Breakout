/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include "game_object.h"
#include "shader.h"
#include "texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

// TODO: Convert to an aggregate type
// Represents a single particle and its state
struct Particle
{
  glm::vec2 Position, Velocity;
  glm::vec4 Color;
  float Life;

  Particle()
      : Position(0.0f)
      , Velocity(0.0f)
      , Color(1.0f)
      , Life(0.0f)
  {
  }
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator
{
public:
  // Constructor
  ParticleGenerator() = default;
  ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);

  // Update all particles
  void Update(float dt,
              GameObject& object,
              unsigned int newParticles,
              glm::vec2 offset = glm::vec2(0.0f, 0.0f));

  // Render all particles
  void Draw();

private:
  // Initializes buffer and vertex attributes
  void init();

  // Returns the first Particle index that's currently unused e.g. Life <= 0.0f
  // or 0 if no particle is currently inactive
  unsigned int firstUnusedParticle();

  // Respawns particle
  void respawnParticle(Particle& particle,
                       GameObject& object,
                       glm::vec2 offset = glm::vec2(0.0f, 0.0f));

  // Data
  // State
  std::vector<Particle> particles {};
  unsigned int amount {};

  // Render state
  Shader shader {};
  Texture2D texture {};
  unsigned int VAO {};
};

#endif
