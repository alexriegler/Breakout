/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "shader.h"
#include "texture.h"

#include <glad/glad.h>

#include <map>
#include <string>

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager
{
public:
  static Shader& LoadShader(const char* vShaderFile,
                            const char* fShaderFile,
                            const char* gShaderFile,
                            std::string name);

  static Shader& GetShader(std::string name);

  // loads (and generates) a texture from file
  static Texture2D& LoadTexture(const std::string& file,
                                bool alpha,
                                const std::string& resourceName = "");
  // retrieves a stored texture
  static Texture2D& GetTexture(const std::string& file);

  // properly de-allocates all loaded resources
  static void Clear();

  // resource storage
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture2D> Textures;

private:
  // private constructor, that is we do not want any actual resource manager
  // objects. Its members and functions should be publicly available (static).
  ResourceManager() {}

  static Shader& loadShaderFromFile(const char* vShaderFile,
                                    const char* fShaderFile,
                                    const char* gShaderFile = nullptr);
  static Texture2D LoadTextureFromFile(const std::string& filepath, bool alpha);
};

#endif