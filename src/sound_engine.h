#pragma once

#ifndef DISABLE_AUDIO
// clang-format off
#include <SFML/Audio.hpp>
// clang-format on
#endif  // !DISABLE_AUDIO

#include <map>
#include <memory>
#include <string>
#include <vector>

class SoundEngine
{
public:
  // Default constructor
  SoundEngine() = default;

  // Copy constructor/assignment operator
  SoundEngine(const SoundEngine&) = delete;
  SoundEngine& operator=(const SoundEngine&) = delete;

  // Move constructor/assignment operator
  SoundEngine(SoundEngine&&) = default;
  SoundEngine& operator=(SoundEngine&&) = default;

  // Functions
  void loadSound(const std::string& filename);
  void play2D(const std::string& filename, bool loop);
  void playMusic(const std::string& filename, bool loop = true);

private:
#ifndef DISABLE_AUDIO
  std::map<std::string, sf::SoundBuffer> m_soundsByName;
  std::vector<sf::Sound> m_sounds;
  std::vector<std::shared_ptr<sf::Music>> m_music;
#endif  // DISABLE_AUDIO
};