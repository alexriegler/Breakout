#include "Shader.h"

#include <format>
#include <string>

namespace ar
{
	/// <summary>
	/// Gets the string representation of the ShaderType enumeration.
	/// </summary>
	/// <param name="shader_type">The ShaderType enumeration to convert to a string</param>
	/// <returns>The string representation of the ShaderType enumeration</returns>
	constexpr auto string_rep(ShaderType shader_type)
	{
		switch (shader_type)
		{
		case ShaderType::vertex:
			return "vertex";
		case ShaderType::fragment:
			return "fragment";
		case ShaderType::geometry:
			return "geometry";
		case ShaderType::invalid:
		default:
			return "invalid";
		}
	}

	// Constructor
	/// <summary>
	/// Constructs a shader from the provided source code with the specified type.
	/// </summary>
	/// <param name="source">The source code of the shader</param>
	/// <param name="type">The type of shader</param>
	Shader::Shader(std::string_view source, ShaderType type)
		: m_type{ type }, m_id{ compile(source) }
	{}

	// Destructor
	Shader::~Shader()
	{
		glDeleteShader(m_id);
	}

	// Move constructor
	Shader::Shader(Shader&& other) noexcept
	{
		*this = std::move(other);
	}

	// Move assignment operator
	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this != &other)
		{
			m_type = std::exchange(other.m_type, ShaderType::invalid);
			m_id = std::exchange(other.m_id, 0);
		}
		return *this;
	}

	/// <summary>
	/// Gets the id of the compiled shader.
	/// </summary>
	/// <returns>Id of the compiled shader</returns>
	Shader::id_type Shader::id() const noexcept
	{
		return m_id;
	}

	/// <summary>
	/// Attaches the shader to the specified program.
	/// </summary>
	/// <param name="program_id">The ID of the program to attach to</param>
	void Shader::attach_to(Shader::id_type program_id)
	{
		// TODO: Add error checking
		glAttachShader(program_id, m_id);
	}

	/// <summary>
	/// Detaches the shader from the specified program.
	/// </summary>
	/// <param name="program_id">The ID of the program to detach from</param>
	void Shader::detach_from(Shader::id_type program_id)
	{
		// TODO: Add error checking
		glDetachShader(program_id, m_id);
	}

	/// <summary>
	/// Compiles a shader from source.
	/// </summary>
	/// <param name="source">The source code of the shader</param>
	/// <returns>The id of the compiled shader</returns>
	Shader::id_type Shader::compile(std::string_view source) const
	{
		if (m_type != ShaderType::invalid)
		{
			const auto id = glCreateShader(to_underlying(m_type));
			const char* csource = source.data();
			glShaderSource(id, 1, &csource, nullptr);
			glCompileShader(id);

			// Check for compiler errors
			int success;
			glGetShaderiv(id, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				// TODO: Get size of message first
				constexpr auto size = 512;
				std::string info_log(size, '\0');
				glGetShaderInfoLog(id, size, nullptr, info_log.data());
				throw std::runtime_error{ std::format("Compilation of {} shader failed:\n{}\n", string_rep(m_type), info_log) };
			}

			return id;
		}
		else
		{
			throw std::runtime_error{ std::format("Invalid shader type.\n") };
		}
	}
}
