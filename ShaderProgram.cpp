#include "ShaderProgram.h"

#include <format>
#include <string>

namespace ar
{
	// Constructors
	/// <summary>
	/// Constructs a shader program from the provided vertex and fragment shaders.
	/// </summary>
	/// <param name="vertex_shader">The vertex shader</param>
	/// <param name="fragment_shader">The fragment shader</param>
	ShaderProgram::ShaderProgram(Shader& vertex_shader, Shader& fragment_shader)
		: m_id{ link(&vertex_shader, &fragment_shader) }
	{}

	/// <summary>
	/// Constructs a shader program from the provided vertex, fragment, and geometry shaders.
	/// </summary>
	/// <param name="vertex_shader">The vertex shader</param>
	/// <param name="fragment_shader">The fragment shader</param>
	/// <param name="geometry_shader">The geometry shader</param>
	ShaderProgram::ShaderProgram(Shader& vertex_shader, Shader& fragment_shader, Shader& geometry_shader)
		: m_id{ link(&vertex_shader, &fragment_shader, &geometry_shader) }
	{}

	// Deconstructor
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}

	// Move constructor
	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	{
		*this = std::move(other);
	}

	// Move assignment operator
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
	{
		if (this != &other)
		{
			m_id = std::exchange(other.m_id, 0);
		}
		return *this;
	}

	// Functions
	/// <summary>
	/// Make the shader the active shader
	/// </summary>
	void ShaderProgram::use()
	{
		// TODO: Error checking
		glUseProgram(m_id);
	}

	// Utility uniform functions
	// TODO: Add caching for uniforms
	void ShaderProgram::set_bool(std::string_view name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.data()), static_cast<int>(value));
	}

	void ShaderProgram::set_int(std::string_view name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_id, name.data()), value);
	}

	void ShaderProgram::set_float(std::string_view name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_id, name.data()), value);
	}

	void ShaderProgram::set_vec2(std::string_view name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
	}

	void ShaderProgram::set_vec2(std::string_view name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_id, name.data()), x, y);
	}

	void ShaderProgram::set_vec3(std::string_view name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
	}

	void ShaderProgram::set_vec3(std::string_view name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_id, name.data()), x, y, z);
	}

	void ShaderProgram::set_vec4(std::string_view name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(m_id, name.data()), 1, glm::value_ptr(value));
	}

	void ShaderProgram::set_vec4(std::string_view name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(m_id, name.data()), x, y, z, w);
	}

	void ShaderProgram::set_mat2(std::string_view name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void ShaderProgram::set_mat3(std::string_view name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void ShaderProgram::set_mat4(std::string_view name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.data()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	ShaderProgram::id_type ShaderProgram::link(gsl::not_null<Shader*> vertex_shader,
											   gsl::not_null<Shader*> fragment_shader,
											   Shader* geometry_shader)
	{
		const auto id = glCreateProgram();

		// Attach shaders
		// TODO: RAII attacher/detacher
		vertex_shader->attach_to(id);
		fragment_shader->attach_to(id);
		if (geometry_shader != nullptr)
		{
			geometry_shader->attach_to(id);
		}

		// Link shader program
		glLinkProgram(id);

		// Check for linking errors
		int success;
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			// TODO: Get size of message first
			constexpr auto size = 512;
			std::string info_log(size, '\0');
			glGetProgramInfoLog(id, size, nullptr, info_log.data());
			throw std::runtime_error{ std::format("Failed to link shader program:\n{0}\n", info_log) };
		}

		// Detach shaders
		if (geometry_shader != nullptr)
		{
			geometry_shader->detach_from(id);
		}
		fragment_shader->detach_from(id);
		vertex_shader->detach_from(id);

		return id;
	}
}
