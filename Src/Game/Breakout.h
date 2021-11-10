#pragma once

#include "Game.h"
#include "Renderer.h"
#include "Window.h"

#include <array>
#include <string>

namespace ar
{
	/// <summary>
	/// Represents the current state of the game
	/// </summary>
	enum class GameState
	{
		active,
		menu,
		win
	};

	//void frame_buffer_size_callback(GLFWwindow*, int, int);

	class Breakout : public Game
	{
	public:
		// Constructor
		Breakout();
		Breakout(int width, int height);

		// Functions
		// Callbacks (friend so that they can access private members)
		friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
		friend void frame_buffer_size_callback(GLFWwindow* /*window*/, int width, int height);

	protected:
		// Game loop
		void fixed_update() override;
		void update() override;
		void late_update() override;
		void render() override;
		void on_game_pause() override;
		
		// State
		bool is_running() const override;
		bool is_paused() const override;

	private:
		// Input
		void process_input();

		// Constants
		static constexpr auto max_keys = 1024;

		// Data
		int m_width{ 800 };
		int m_height{ 600 };
		Window m_window{ "Breakout", m_width, m_height };
		GameState m_state{ GameState::active };
		float m_delta_time{ 0.0f };	// Time between current frame and last frame
		float m_last_frame{ 0.0f };	// Time of last frame
		std::array<bool, max_keys> m_keys{};
	};

	// Callbacks
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void frame_buffer_size_callback(GLFWwindow* /*window*/, int width, int height);
}