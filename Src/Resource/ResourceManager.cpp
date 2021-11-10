#include "ResourceManager.h"

#include "ImageResource.h"

#include <filesystem>
#include <format>
#include <fstream>

namespace ar
{
	/// <summary>
	/// Gets the resource manager instance.
	/// </summary>
	/// <returns>The resource manager instance</returns>
	ResourceManager& ResourceManager::instance()
	{
		static ResourceManager singleton{};
		return singleton;
	}

	/// <summary>
	/// Generates a new shader program with the specified name from a vertex,
	/// fragment, and optional geometry shader source file and returns that shader
	/// program. The shader program is stored by name for future reference.
	/// </summary>
	/// <param name="name">The new name for the shader program</param>
	/// <param name="vertex_shader_path">The file path of the vertex shader source code</param>
	/// <param name="fragment_shader_path">The file path of the fragment shader source code</param>
	/// <param name="geometry_shader_path">The file path of the geometry shader source code</param>
	/// <returns>A reference to the generated shader program</returns>
	ShaderProgram& ResourceManager::load_shader_program(
		std::string_view name,
		std::string_view vertex_shader_path,
		std::string_view fragment_shader_path,
		std::string_view geometry_shader_path)
	{
		auto shader_program = load_shader_program_from_file(
			vertex_shader_path, fragment_shader_path, geometry_shader_path);
		auto sp_ptr = std::make_unique<ShaderProgram>(std::move(shader_program));
		auto [it, success] = m_shader_programs.insert({ std::string{name}, std::move(sp_ptr) });
		if (success)
		{
			return *(it->second);
		}
		else
		{
			throw std::runtime_error(std::format("A shader program with name \"{0}\" already exists.", name));
		}
	}

	/// <summary>
	/// Returns a reference to the shader program with the specified name. If no
	/// such shader program exists, an exception of type std::out_of_range is thrown.
	/// </summary>
	/// <param name="name">The name of the shader program to get</param>
	/// <returns>A reference to the shader program requested</returns>
	ShaderProgram& ResourceManager::get_shader_program(const std::string& name) const
	{
		try
		{
			auto& sp_ptr = m_shader_programs.at(name);
			return *sp_ptr;
		}
		catch (const std::out_of_range&)
		{
			throw std::out_of_range(std::format("A shader program with name \"{0}\" does not exist.", name));
		}
	}

	/// <summary>
	/// Generates a new texture with the specified name and the data at the path
	/// and returns that texture. The texture is stored by name for future reference.
	/// </summary>
	/// <param name="name">The new name for the texture</param>
	/// <param name="texture_path">The file path of the texture data</param>
	/// <param name="format">The format of the texture data</param>
	/// <returns>A reference to the generated texture </returns>
	Texture2D& ResourceManager::load_texture(std::string_view name, std::string_view texture_path, TextureFormat format)
	{
		auto texture = Texture2D{ ImageResource{ texture_path }, format };
		auto t_ptr = std::make_unique<Texture2D>(std::move(texture));
		auto [it, success] = m_textures.insert({ std::string{name}, std::move(t_ptr) });
		if (success)
		{
			return *(it->second);
		}
		else
		{
			throw std::runtime_error(std::format("A texture with name \"{0}\" already exists.", name));
		}
	}

	// retrieves a stored texture

	/// <summary>
	/// Returns a reference to the texture with the specified name. If no
	/// such texture exists, an exception of type std::out_of_range is thrown.
	/// </summary>
	/// <param name="name">The name of the texture to get</param>
	/// <returns>A reference to the texture requested</returns>
	Texture2D& ResourceManager::get_texture(const std::string& name) const
	{
		try
		{
			auto& t_ptr = m_textures.at(name);
			return *t_ptr;
		}
		catch (const std::out_of_range&)
		{
			throw std::out_of_range(std::format("A texture with name \"{0}\" does not exist.", name));
		}
	}

	// properly de-allocates all loaded resources
	void ResourceManager::clear()
	{
		// TODO: Implement. Probably map function.
	}

	/// <summary>
	/// Returns a std::string that contains the data from the specified path.
	/// </summary>
	/// <param name="path">The file path of the data</param>
	/// <returns>The std::string containing the data</returns>
	std::string ResourceManager::file_to_string(std::string_view path) const
	{
		std::ifstream file{};
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			file.open(path.data());
			std::string str{};
			str.reserve(std::filesystem::file_size(path));
			str.assign(
				std::istreambuf_iterator<char>{ file },
				std::istreambuf_iterator<char>{}
			);
			return str;
		}
		catch (const std::ifstream::failure&)
		{
			throw std::runtime_error{ std::format("Failed to read file {0}.", path) };
		}
	}

	/// <summary>
	/// Generates a shader program from the provided shader paths and returns it.
	/// </summary>
	/// <param name="vertex_shader_path">The file path of the vertex shader</param>
	/// <param name="fragment_shader_path">The file path of the fragment shader</param>
	/// <param name="geometry_shader_path">The file path of the geometry shader</param>
	/// <returns></returns>
	ShaderProgram ResourceManager::load_shader_program_from_file(
		std::string_view vertex_shader_path,
		std::string_view fragment_shader_path,
		std::string_view geometry_shader_path)
	{
		auto vertex_shader = VertexShader{ file_to_string(vertex_shader_path) };
		auto fragment_shader = FragmentShader{ file_to_string(fragment_shader_path) };
		if (!geometry_shader_path.empty())
		{
			auto geometry_shader = GeometryShader{ file_to_string(geometry_shader_path) };
			return ShaderProgram(vertex_shader, fragment_shader, geometry_shader);
		}
		else
		{
			return ShaderProgram(vertex_shader, fragment_shader);
		}
	}
}