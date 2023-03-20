#include "sound_engine.h"

#ifndef DISABLE_AUDIO

void SoundEngine::loadSound(const std::string& filename)
{
  sf::SoundBuffer buffer {};
  if (not buffer.loadFromFile(filename)) {
    throw std::runtime_error {"Unable to load file"};
  }
  auto [it, success] = m_soundsByName.insert({filename, buffer});
  if (not success) {
    throw std::runtime_error {"Sound file already loaded"};
  }
}

void SoundEngine::play2D(const std::string& filename, bool loop)
{
  auto search = m_soundsByName.find(filename);
  if (search == m_soundsByName.end()) {
    throw std::runtime_error {"File not yet loaded"};
  }

  if (m_sounds.size() > 200) {
    throw std::runtime_error {"Too many sounds"};
  }

  // TODO: Utilize circular buffer
  auto& sound = m_sounds.emplace_back(search->second);
  sound.play();
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
