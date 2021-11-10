#include "Window.h"

#include <gsl/gsl>

#include <stdexcept>

namespace ar
{
	// Constructor
	Window::Window(std::string_view title, int width, int height, bool resizable)
		: m_title{ title }, m_width{ width }, m_height{ height }
	{
		Expects(width > 0 && height > 0);
		init_libraries();
		glfwWindowHint(GLFW_RESIZABLE, resizable);
		m_window = create_window();
		load_functions();

		// TODO: Abstract away
		// Configuration
		glViewport(0, 0, m_width, m_height);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Destructor
	Window::~Window()
	{
		close();
		glfwTerminate();
	}

	// Move constructor
	Window::Window(Window&& other) noexcept
	{
		*this = std::move(other);
	}

	// Move assignment operator
	Window& Window::operator=(Window && other) noexcept
	{
		if (this != &other)
		{
			m_title = std::move(other.m_title);
			m_width = std::exchange(other.m_width, 0);
			m_height = std::exchange(other.m_height, 0);
			m_window = std::exchange(other.m_window, nullptr);
		}
		return *this;
	}

	// Functions
	/// <summary>
	/// Closes the window.
	/// </summary>
	void Window::close() const noexcept
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	/// <summary>
	/// Returns true if the window should close; false, otherwise.
	/// </summary>
	/// <returns>True, if the window should close; false, otherwise</returns>
	bool Window::should_close() const noexcept
	{
		return glfwWindowShouldClose(m_window);
	}

	/// <summary>
	/// Gets the current time.
	/// </summary>
	/// <returns>The current time</returns>
	double Window::time() const
	{
		const auto now = glfwGetTime();
		if (now == 0)
		{
			throw std::runtime_error{ "Unable to get the current time." };
		}
		return now;
	}

	/// <summary>
	/// Checks if the key has been pressed.
	/// </summary>
	/// <param name="key">The key to check</param>
	/// <returns>True, if the key has been pressed; false, otherwise</returns>
	bool Window::key_pressed(int key) const noexcept
	{
		// TODO: Use enum class for keys.
		return glfwGetKey(m_window, key) == GLFW_PRESS;
	}

	/// <summary>
	/// Gets the native pointer for this window.
	/// </summary>
	/// <returns>the native pointer for this window</returns>
	Window::pointer Window::native_ptr() const noexcept
	{
		return m_window;
	}

	/// <summary>
	/// Gets the aspect ratio of the window.
	/// </summary>
	/// <returns>The aspect ratio of the window</returns>
	constexpr float Window::aspect_ratio() const noexcept
	{
		return static_cast<float>(m_width) / m_height;
	}

	/// <summary>
	/// Initializes required libraries.
	/// </summary>
	void Window::init_libraries() const
	{
		const auto status = glfwInit();
		if (status == GLFW_FALSE)
		{
			throw std::runtime_error{ "Failed to initialize GLFW library" };
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	/// <summary>
	/// Creates a window and makes its context current. If creating the window fails,
	/// throws std::runtime_error.
	/// </summary>
	/// <returns>The handle of the created window</returns>
	Window::pointer Window::create_window() const
	{
		auto window = glfwCreateWindow(m_width, m_height, m_title.data(), nullptr, nullptr);
		if (!window)
		{
			throw std::runtime_error{ "Failed to create GLFW window" };
		}
		glfwMakeContextCurrent(window);
		return window;
	}

	/// <summary>
	/// Loads all library function pointers.
	/// </summary>
	void Window::load_functions() const
	{
		// Initialize glad to load all OpenGL function pointers
		// TODO: Change cast to static_cast
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::runtime_error{ "Failed to initialize GLAD" };
		}
	}
}