#pragma once

#include "ImageResource.h"
#include "Renderer.h"

namespace ar
{
	enum class TextureWrapping
	{
		repeat,
		mirrored_repeat,
		clamp_to_edge,
		clamp_to_border
	};

	enum class TextureFiltering
	{
		nearest,
		linear,
		nearest_mipmap_nearest,
		linear_mipmap_nearest,
		nearest_mipmap_linear,
		linear_mipmap_linear
	};

	enum class TextureFormat
	{
		red = GL_RED,
		rg = GL_RG,
		rgb = GL_RGB,
		bgr = GL_BGR,
		rgba = GL_RGBA,
		bgra = GL_BGRA
	};

	enum class TextureUnit
	{
		none = -1,
		zero = GL_TEXTURE0,
		one = GL_TEXTURE1
	};

	/// <summary>
	/// Class for 2D textures.
	/// </summary>
	class Texture2D
	{
	public:
		// Aliases
		using ID_type = GLuint;

		// Constructor
		Texture2D(
			int width,
			int height,
			std::byte* data,
			TextureFormat format = TextureFormat::rgb,
			TextureWrapping wrapping = TextureWrapping::repeat,
			TextureFiltering filtering = TextureFiltering::linear);

		Texture2D(
			const ImageResource& image,
			TextureFormat format = TextureFormat::rgb,
			TextureWrapping wrapping = TextureWrapping::repeat,
			TextureFiltering filtering = TextureFiltering::linear);

		// Deconstructor
		~Texture2D();

		// Copy constructor & assignment operator
		Texture2D(const Texture2D& other) = delete;
		Texture2D& operator=(const Texture2D& other) = delete;

		// Move constructor & assignment operator
		Texture2D(Texture2D&& other) noexcept;
		Texture2D& operator=(Texture2D&& other) noexcept;

		// Functions
		[[nodiscard]] ID_type id() const noexcept;
		void bind();
		void unbind();

	private:
		void set_wrapping(TextureWrapping wrapping);
		void set_filtering(TextureFiltering filtering);

		// Data
		ID_type m_id{ 0 };
		int m_width{ 0 };
		int m_height{ 0 };
	};
}