#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace Location
{
/**
 * @brief The location of all texture files for the application.
 */
static const std::string textureDirectory = "textures";

/**
 * @brief Returns the path to a texture.
 * @param filename The name of the texture file.
 * @return The path of the texture relative to the main executable.
 */
fs::path PathToTexture(const std::string& filename)
{
  return fs::path(textureDirectory) / filename;
}
}  // namespace Location