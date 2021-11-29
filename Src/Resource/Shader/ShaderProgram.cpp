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

	/// <summary>
	/// Links the shaders together to create a shader program.
	/// </summary>
	/// <param name="vertex_shader">The vertex shader</param>
	/// <param name="fragment_shader">The fragment shader</param>
	/// <param name="geometry_shader">Optional. The geometry shader</param>
	/// <returns></returns>
	ShaderProgram::ID_type ShaderProgram::link(gsl::not_null<Shader*> vertex_shader,
											   gsl::not_null<Shader*> fragment_shader,
											   Shader* geometry_shader)
	{
		const auto id = glCreateProgram();
		const std::array shaders{ static_cast<Shader*>(vertex_shader),
								  static_cast<Shader*>(fragment_shader),
								  geometry_shader };
		auto attacher = Attacher( id, shaders );
		glLinkProgram(id);
		check_for_linking_errors(id);
		return id;
	}

	/// <summary>
	/// Checks for shader linking errors and throws an error if found.
	/// </summary>
	/// <param name="id">The ID of the shader program</param>
	void ShaderProgram::check_for_linking_errors(ID_type id)
	{
		GLint is_linked = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint max_length = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length);
			std::basic_string<GLchar> info_log{};
			info_log.resize(max_length);
			glGetProgramInfoLog(id, max_length, &max_length, info_log.data());
			throw std::runtime_error{
				std::format("Failed to link shader program:\n{}\n", info_log)
			};
		}
	}
}
