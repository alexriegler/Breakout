#pragma once

#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include <memory>
#include <unordered_map>
#include <string_view>

namespace ar
{
	/// <summary>
	/// A resource manager class that has several functions to load resources,
	/// such as textures and shader programs. Loaded resources are stored for
	/// future reference by their string name.
	/// </summary>
	class ResourceManager
	{
	public:
		// TODO: Add copy and move operations

		// Instance accessor
		static ResourceManager& instance();

		// Functions
		// Shader program
		ShaderProgram& load_shader_program(
			std::string_view name,
			std::string_view vertex_shader_path,
			std::string_view fragment_shader_path,
			std::string_view geometry_shader_path = {});
		ShaderProgram& get_shader_program(const std::string& name) const;

		// Texture
		Texture2D& load_texture(std::string_view name, std::string_view texture_path, TextureFormat format);
		Texture2D& get_texture(const std::string& name) const;

		void clear();

	private:
		// Constructor (private to keep only single instance)
		ResourceManager() = default;

		std::string file_to_string(std::string_view path) const;
		ShaderProgram load_shader_program_from_file(
			std::string_view vertex_shader_path,
			std::string_view fragment_shader_path,
			std::string_view geometry_shader_path = {});

		// Data
		std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> m_shader_programs{};
		std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_textures{};
	};
}