/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "resource_manager.h"

#include "resource_location.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

// loads (and generates) a shader program from file loading vertex, fragment
// (and geometry) shader's source code. If gShaderFile is not nullptr, it also
// loads a geometry shader
Shader& ResourceManager::LoadShader(const char* vShaderFile,
                                    const char* fShaderFile,
                                    const char* gShaderFile,
                                    std::string name)
{
  Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
  return Shaders[name];
}

// retrieves a stored shader
Shader& ResourceManager::GetShader(std::string name)
{
  return Shaders[name];
}

/**
 * @brief Loads the texture with the specified name from the textures directory.
 *
 * If a resource name is not provided, then it will default to the name of the
 * file without its extension.
 *
 * @param filename      The filename of the texture to load.
 * @param alpha         true, if the texture uses the alpha channel; false,
 *                      otherwise.
 * @param resourceName  The name to use for the loaded resource.
 * @return The loaded texture.
 */
Texture2D& ResourceManager::LoadTexture(const std::string& filename,
                                        bool alpha,
                                        const std::string& resourceName)
{
  const auto filepath = Location::PathToTexture(filename);
  if (fs::exists(filepath)) {
    const auto name =
        resourceName.empty() ? filepath.stem().string() : resourceName;
    const auto texture = LoadTextureFromFile(filepath.string(), alpha);
    auto [it, success] = Textures.insert({name, texture});
    if (success) {
      return it->second;
    } else {
      throw std::runtime_error {
          std::format("A texture with name \"{}\" already exists", name)};
    }
  } else {
    throw fs::filesystem_error {
        std::format("Texture not found: {}", filepath.string()), filepath, {}};
  }
}

/**
 * @brief Tries to get the texture with the specified name.
 * @param name The name of the texture resource.
 * @return The texture with the specified name.
 */
Texture2D& ResourceManager::GetTexture(const std::string& name)
{
  auto it = Textures.find(name);
  if (it != Textures.end()) {
    return it->second;
  } else {
    throw std::runtime_error {
        std::format("Texture with name \"{}\" does not exist", name)};
  }
}

void ResourceManager::Clear()
{
  // (properly) delete all shaders
  for (auto iter : Shaders)
    glDeleteProgram(iter.second.ID);
  // (properly) delete all textures
  for (auto iter : Textures)
    glDeleteTextures(1, &iter.second.ID);
}

Shader& ResourceManager::loadShaderFromFile(const char* vShaderFile,
                                            const char* fShaderFile,
                                            const char* gShaderFile)
{
  // TODO: Check if file exists
  // 1. retrieve the vertex/fragment source code from filePath
  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;
  try {
    // open files
    std::ifstream vertexShaderFile(vShaderFile);
    std::ifstream fragmentShaderFile(fShaderFile);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();
    // close file handlers
    vertexShaderFile.close();
    fragmentShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    // if geometry shader path is present, also load a geometry shader
    if (gShaderFile != nullptr) {
      std::ifstream geometryShaderFile(gShaderFile);
      std::stringstream gShaderStream;
      gShaderStream << geometryShaderFile.rdbuf();
      geometryShaderFile.close();
      geometryCode = gShaderStream.str();
    }
  } catch (std::exception e) {
    std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
  }
  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();
  const char* gShaderCode = geometryCode.c_str();
  // 2. now create shader object from source code
  Shader shader;
  shader.Compile(
      vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
  return shader;
}

/**
 * @brief Creates a texture object from the specified file.
 * @param filepath  The path to the source file.
 * @param alpha     true, if the texture uses the alpha channel; false,
 *                  otherwise.
 * @return The created texture object.
 */
Texture2D ResourceManager::LoadTextureFromFile(const std::string& filepath,
                                               bool alpha)
{
  // create texture object
  Texture2D texture;
  if (alpha) {
    texture.Internal_Format = GL_RGBA;
    texture.Image_Format = GL_RGBA;
  }
  // load image
  int width, height, nrChannels;
  unsigned char* data =
      stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
  // now generate texture
  texture.Generate(width, height, data);
  // and finally free image data
  stbi_image_free(data);
  return texture;
}
