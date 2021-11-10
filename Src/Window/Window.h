#pragma once

#include "Renderer.h"

#include <stdexcept>
#include <string_view>

namespace ar
{
	class Window
	{
	public:
		// Type aliases
		using pointer = GLFWwindow*;

		// Constructor
		Window() = default;
		Window(std::string_view title, int width, int height, bool resizable = false);

		// Destructor
		~Window();

		// Copy constructor & assignment operator
		Window(const Window& other) = delete;
		Window& operator=(const Window& other) = delete;

		// Move constructor & assignment operator
		Window(Window&& other) noexcept;
		Window& operator=(Window&& other) noexcept;

		// Functions
		// TODO: Add set callback function
		void close() const noexcept;

		[[nodiscard]] bool should_close() const noexcept;
		[[nodiscard]] double time() const;
		[[nodiscard]] bool key_pressed(int key) const noexcept;
		[[nodiscard]] pointer native_ptr() const noexcept;
		[[nodiscard]] constexpr float aspect_ratio() const noexcept;

	private:
		void init_libraries() const;
		[[nodiscard]] pointer create_window() const;
		void load_functions() const;

		// Data
		std::string m_title{ "OpenGL Window" };
		int m_width{ 800 };
		int m_height{ 600 };
		pointer m_window{ nullptr };
	};
}