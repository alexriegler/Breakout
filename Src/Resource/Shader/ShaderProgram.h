#pragma once

#include "Renderer.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gsl/gsl>

#include <string_view>

namespace ar
{
	/// <summary>
	/// Represents a shader program consisting of a vertex shader, fragment
	/// shader, and, optionally, a geometry shader.
	/// </summary>
	class ShaderProgram
	{
	public:
		// Aliases
		using ID_type = GLuint;

		// Constructors
		ShaderProgram(Shader& vertex_shader, Shader& fragment_shader);
		ShaderProgram(Shader& vertex_shader, Shader& fragment_shader, Shader& geometry_shader);

		// Deconstructor
		~ShaderProgram();

		// Copy constructor & assignment operator
		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;

		// Move constructor & assignment operator
		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;

		// Functions
		void use();

		// Utility uniform functions
		void set_bool(std::string_view name, bool value) const;
		void set_int(std::string_view name, int value) const;
		void set_float(std::string_view name, float value) const;
		
		void set_vec2(std::string_view name, const glm::vec2& value) const;
		void set_vec2(std::string_view name, float x, float y) const;
		
		void set_vec3(std::string_view name, const glm::vec3& value) const;
		void set_vec3(std::string_view name, float x, float y, float z) const;
		
		void set_vec4(std::string_view name, const glm::vec4& value) const;
		void set_vec4(std::string_view name, float x, float y, float z, float w) const;
		
		void set_mat2(std::string_view name, const glm::mat2& mat) const;
		
		void set_mat3(std::string_view name, const glm::mat3& mat) const;
		
		void set_mat4(std::string_view name, const glm::mat4& mat) const;

	private:
		ID_type link(gsl::not_null<Shader*> vertex_shader,
				     gsl::not_null<Shader*> fragment_shader,
				     Shader* geometry_shader = nullptr);

		// Data
		ID_type m_id{ 0 };
	};
}
