#pragma once

#include "Renderer.h"
#include "Utilities.h"

#include <string_view>

namespace ar
{
	/// <summary>
	/// Represents the different types of shaders.
	/// </summary>
	enum class ShaderType
	{
		invalid = GL_FALSE,
		vertex = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
		geometry = GL_GEOMETRY_SHADER
	};

	constexpr auto string_rep(ShaderType shader_type);

	/// <summary>
	/// Represents a shader.
	/// </summary>
	class Shader
	{
	public:
		// Aliases
		using ID_type = GLuint;

		// Constructor
		Shader(std::string_view source, ShaderType type);
		
		// Destructor
		~Shader();

		// Copy constructor & assignment operator
		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;

		// Move constructor & assignment operator
		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		// Functions
		[[nodiscard]] ID_type id() const noexcept;
		void attach_to(ID_type program_id);
		void detach_from(ID_type program_id);

	private:
		[[nodiscard]] ID_type compile(std::string_view source) const;
		static void check_for_errors(ID_type id, ShaderType type);

		// Data
		ShaderType m_type{ ShaderType::invalid };
		ID_type m_id{ 0 };
	};

	/// <summary>
	/// Represents a vertex shader.
	/// </summary>
	class VertexShader : public Shader
	{
	public:
		/// <summary>
		/// Constructs a vertex shader from the provided source code.
		/// </summary>
		/// <param name="source">The source code of the shader</param>
		VertexShader(std::string_view source)
			: Shader(source, ShaderType::vertex)
		{}
	};

	/// <summary>
	/// Represents a fragment shader.
	/// </summary>
	class FragmentShader : public Shader
	{
	public:
		/// <summary>
		/// Constructs a fragment shader from the provided source code.
		/// </summary>
		/// <param name="source">The source code of the shader</param>
		FragmentShader(std::string_view source)
			: Shader(source, ShaderType::fragment)
		{}
	};

	/// <summary>
	/// Represents a geometry shader.
	/// </summary>
	class GeometryShader : public Shader
	{
	public:
		/// <summary>
		/// Constructs a geometry shader from the provided source code.
		/// </summary>
		/// <param name="source">The source code of the shader</param>
		GeometryShader(std::string_view source)
			: Shader(source, ShaderType::geometry)
		{}
	};
}