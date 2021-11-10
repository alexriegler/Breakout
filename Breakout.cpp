#include "Breakout.h"

namespace ar
{
	// Constructor
	Breakout::Breakout()
	{
		// Set window user pointer (required for callbacks)
		glfwSetWindowUserPointer(m_window.native_ptr(), this);

		// Set callbacks
		glfwSetKeyCallback(m_window.native_ptr(), key_callback);
		glfwSetFramebufferSizeCallback(m_window.native_ptr(), frame_buffer_size_callback);
	}

	Breakout::Breakout(int width, int height)
		: m_width{ width }, m_height{ height }
	{
		// Set window user pointer (required for callbacks)
		glfwSetWindowUserPointer(m_window.native_ptr(), this);

		// Set callbacks
		glfwSetKeyCallback(m_window.native_ptr(), key_callback);
		glfwSetFramebufferSizeCallback(m_window.native_ptr(), frame_buffer_size_callback);
	}

	// Functions
	void Breakout::fixed_update() {}

	void Breakout::update()
	{
		// Per-frame time logic
		// TODO: Add timef() function
		const auto current_frame = static_cast<float>(m_window.time());
		m_delta_time = current_frame - m_last_frame;
		m_last_frame = current_frame;

		// Input
		process_input();
	}

	void Breakout::late_update() {}

	void Breakout::render()
	{
		// Background
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Check and call events and swap the buffers
		glfwSwapBuffers(m_window.native_ptr());
		glfwPollEvents();
	}

	void Breakout::on_game_pause() {}

	bool Breakout::is_running() const
	{
		return !m_window.should_close();
	}

	bool Breakout::is_paused() const
	{
		// NOTE: Not implemented
		return false;
	}

	void Breakout::process_input()
	{}

	// Callbacks
	void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
	{
		Breakout* handle = static_cast<Breakout*>(glfwGetWindowUserPointer(window));
		if (handle)
		{
			// When the escape key is pressed, close the window
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				handle->m_window.close();
			}
			if (key >= 0 && key < Breakout::max_keys)
			{
				if (action == GLFW_PRESS)
				{
					handle->m_keys[key] = true;
				}
				else if (action == GLFW_RELEASE)
				{
					handle->m_keys[key] = false;
				}
			}
		}
	}

	void frame_buffer_size_callback(GLFWwindow* /*window*/, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}