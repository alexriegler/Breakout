#include "Texture2D.h"

#include "Utilities.h"

#include <gsl/gsl>

#include <stdexcept>

#include <cstddef>

namespace ar
{
	// Constructor
	/// <summary>
	/// Constructs a 2D texture with the specified dimensions and data.
	/// </summary>
	/// <param name="width">The width of the texture</param>
	/// <param name="height">The height of the texture</param>
	/// <param name="data">The pixel data of the texture</param>
	/// <param name="format">The format of the texture</param>
	/// <param name="wrapping">The wrapping style for the texture</param>
	/// <param name="filtering">The filtering style for the texture</param>
	Texture2D::Texture2D(
		int width,
		int height,
		std::byte* data,
		TextureFormat format,
		TextureWrapping wrapping,
		TextureFiltering filtering)
		: m_width{ width }, m_height{ height }
	{
		Expects(height > 0 && width > 0);

		glGenTextures(1, &m_id);
		
		glBindTexture(GL_TEXTURE_2D, m_id);
		
		set_wrapping(wrapping);
		set_filtering(filtering);
		
		// Load data
		glTexImage2D(
			GL_TEXTURE_2D, 0, to_underlying(format),
			m_width, m_height, 0, to_underlying(format), GL_UNSIGNED_BYTE, data
		);

		// Generate mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
		
		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/// <summary>
	/// Constructs a 2D texture from the image.
	/// </summary>
	/// <param name="image">The image for the texture</param>
	/// <param name="format">The format of the texture</param>
	/// <param name="wrapping">The wrapping style for the texture</param>
	/// <param name="filtering">The filtering style for the texture</param>
	Texture2D::Texture2D(
		const ImageResource& image,
		TextureFormat format,
		TextureWrapping wrapping,
		TextureFiltering filtering)
		: Texture2D{ image.width(), image.height(), image.data(), format, wrapping, filtering }
	{}

	// Deconstructor
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_id);
	}

	// Move constructor
	Texture2D::Texture2D(Texture2D&& other) noexcept
	{
		*this = std::move(other);
	}

	// Move assignment operator
	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
	{
		if (this != &other)
		{
			m_id = std::exchange(other.m_id, 0);
			m_width = std::exchange(other.m_width, 0);
			m_height = std::exchange(other.m_height, 0);
		}
		return *this;
	}

	// Functions
	/// <summary>
	/// Gets the id of the texture.
	/// </summary>
	/// <returns>Id of the texture</returns>
	Texture2D::ID_type Texture2D::id() const noexcept
	{
		return m_id;
	}

	/// <summary>
	/// Binds the texture to the active texture unit.
	/// </summary>
	void Texture2D::bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	/// <summary>
	/// Unbinds the the currently bound texture.
	/// </summary>
	void Texture2D::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::set_wrapping(TextureWrapping wrapping)
	{
		switch (wrapping)
		{
		case TextureWrapping::repeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case TextureWrapping::mirrored_repeat:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		case TextureWrapping::clamp_to_edge:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case TextureWrapping::clamp_to_border:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			break;
		default:
			throw std::runtime_error{ "Unexpected wrapping parameter" };
			break;
		}
	}

	void Texture2D::set_filtering(TextureFiltering filtering)
	{
		switch (filtering)
		{
		case TextureFiltering::nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case TextureFiltering::linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case TextureFiltering::nearest_mipmap_nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			break;
		case TextureFiltering::linear_mipmap_nearest:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case TextureFiltering::nearest_mipmap_linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			break;
		case TextureFiltering::linear_mipmap_linear:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		default:
			throw std::runtime_error{ "Unexpected filtering parameter" };
			break;
		}
	}
}