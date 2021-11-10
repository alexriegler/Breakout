#include "ImageResource.h"

#include <stdexcept>
#include <type_traits>

import stb_image;

namespace ar
{
	// Constructor
	ImageResource::ImageResource(std::string_view path, bool flip_vertically_on_load)
	{
		stb::set_flip_vertically_on_load(flip_vertically_on_load);
		static_assert(std::is_same_v<std::underlying_type_t<std::byte>, unsigned char>);
		m_data = reinterpret_cast<std::byte*>(stb::load(path.data(), &m_width, &m_height, &m_channels, 0));
		if (!m_data)
		{
			throw std::runtime_error{ "Failed to load image" };
		}
	}

	// Deconstructor
	ImageResource::~ImageResource()
	{
		stb::image_free(m_data);
	}

	// Move constructor
	ImageResource::ImageResource(ImageResource && other) noexcept
	{
		*this = std::move(other);
	}

	// Move assignment operator
	ImageResource& ImageResource::operator=(ImageResource && other) noexcept
	{
		if (this != &other)
		{
			m_width = std::exchange(other.m_width, 0);
			m_height = std::exchange(other.m_height, 0);
			m_channels = std::exchange(other.m_channels, 0);
			m_data = std::exchange(other.m_data, nullptr);
		}
		return *this;
	}

	/// <summary>
	/// Returns the width of the image.
	/// </summary>
	/// <returns>The width of the image</returns>
	int ImageResource::width() const noexcept
	{
		return m_width;
	}

	/// <summary>
	/// Returns the height of the image.
	/// </summary>
	/// <returns>The height of the image</returns>
	int ImageResource::height() const noexcept
	{
		return m_height;
	}

	/// <summary>
	/// Returns the number of channels in the image.
	/// </summary>
	/// <returns>The number of channels in the image</returns>
	int ImageResource::channels() const noexcept
	{
		return m_channels;
	}

	/// <summary>
	/// Returns a pointer to the underlying byte array.
	/// </summary>
	/// <returns>A pointer to the underlying byte array</returns>
	std::byte* ImageResource::data() const noexcept
	{
		return m_data;
	}
}
