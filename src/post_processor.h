/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "shader.h"
#include "sprite_renderer.h"
#include "texture.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

// TODO: Fix member variable initialization
// PostProcessor hosts all PostProcessing effects for the Breakout
// Game. It renders the game on a textured quad after which one can
// enable specific effects by enabling either the Confuse, Chaos or
// Shake boolean.
// It is required to call BeginRender() before rendering the game
// and EndRender() after rendering the game for the class to work.
class PostProcessor
{
public:
  // Constructor
  PostProcessor() = default;
  PostProcessor(Shader shader, unsigned int width, unsigned int height);

  // Prepares the postprocessor's framebuffer operations before rendering the
  // game
  void BeginRender();
  // Should be called after rendering the game, so it stores all the rendered
  // data into a texture object
  void EndRender();
  // Renders the PostProcessor texture quad (as a screen-encompassing large
  // sprite)
  void Render(float time);

  // State
  Shader PostProcessingShader;
  Texture2D Texture;
  unsigned int Width, Height;

  // options
  bool Confuse, Chaos, Shake;

private:
  // Initialize quad for rendering postprocessing texture
  void initRenderData();

  // Render state
  unsigned int MSFBO, FBO;  // MSFBO = Multisampled FBO. FBO is regular, used
                            // for blitting MS color-buffer to texture
  unsigned int RBO;  // RBO is used for multisampled color buffer
  unsigned int VAO;
};

#endif
