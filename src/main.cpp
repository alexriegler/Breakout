/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "game.h"
#include "resource_manager.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mode);
void error_callback(int error, const char* description);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, false);

  glfwSetErrorCallback(error_callback);

  GLFWwindow* window = glfwCreateWindow(
      SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create window\n";
    return -1;
  }
  glfwMakeContextCurrent(window);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  // OpenGL configuration
  // --------------------
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Requires OpenGL to be initialized
  Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

  // Set pointer for callback to call Breakout methods
  glfwSetWindowUserPointer(window, &Breakout);

  // Callbacks
  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // initialize game
  // ---------------
  Breakout.Init();

  // deltaTime variables
  // -------------------
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    // calculate delta time
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glfwPollEvents();

    // manage user input
    // -----------------
    Breakout.ProcessInput(deltaTime);

    // update game state
    // -----------------
    Breakout.Update(deltaTime);

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    Breakout.Render();

    glfwSwapBuffers(window);
  }

  // delete all resources as loaded using the resource manager
  // ---------------------------------------------------------
  ResourceManager::Clear();

  glfwTerminate();
  return 0;
}

void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mode)
{
  Game& game = *(static_cast<Game*>(glfwGetWindowUserPointer(window)));

  // when a user presses the escape key, we set the WindowShouldClose property
  // to true, closing the application
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS)
      game.Keys[key] = true;
    else if (action == GLFW_RELEASE) {
      game.Keys[key] = false;
      game.KeysProcessed[key] = false;
    }
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void error_callback(int error, const char* description)
{
  std::cerr << "GLFW Error " << error << ": " << description << '\n';
}
