#include "sound_engine.h"

#ifndef DISABLE_AUDIO

void SoundEngine::loadSound(const std::string& filename)
{
  sf::SoundBuffer buffer {};
  if (not buffer.loadFromFile(filename)) {
    throw std::runtime_error {"Unable to load file"};
  }
  auto [it, success] = m_soundBuffersByName.insert({filename, buffer});
  if (not success) {
    throw std::runtime_error {"Sound file already loaded"};
  }
}

void SoundEngine::play2D(const std::string& filename, bool loop)
{
  auto search = m_soundBuffersByName.find(filename);
  if (search == m_soundBuffersByName.end()) {
    throw std::runtime_error {"File not yet loaded"};
  }

  auto [it, _] = m_soundsByName.emplace(filename, search->second);
  it->second.play();
}

void SoundEngine::playMusic(const std::string& filename, bool loop)
{
  auto music_ptr = std::make_shared<sf::Music>();
  if (!music_ptr->openFromFile(filename)) {
    throw std::runtime_error {"File not found"};
  }
  music_ptr->setLoop(loop);
  music_ptr->play();
  m_music.push_back(music_ptr);
}
#else
// If audio is disabled, the following functions are just no-ops
void SoundEngine::loadSound(const std::string& filename) {}

void SoundEngine::play2D(const std::string& filename, bool loop) {}

void SoundEngine::playMusic(const std::string& filename, bool loop) {}
#endif  // ! DISABLE_AUDIO
