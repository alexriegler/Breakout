#pragma once

#include <string_view>

#include <cstddef>

namespace ar
{
	/// <summary>
	/// Class for an image resource. Manages the lifetime of the image data.
	/// </summary>
	class ImageResource
	{
	public:
		// Constructor
		ImageResource(std::string_view path, bool flip_vertically_on_load = true);

		// Deconstructor
		~ImageResource();

		// Copy constructor & assignment operator
		ImageResource(const ImageResource& other) = delete;
		ImageResource& operator=(const ImageResource& other) = delete;

		// Move constructor & assignment operator
		ImageResource(ImageResource&& other) noexcept;
		ImageResource& operator=(ImageResource&& other) noexcept;

		// Functions
		int width() const noexcept;
		int height() const noexcept;
		int channels() const noexcept;
		std::byte* data() const noexcept;

	private:
		int m_width{ 0 };
		int m_height{ 0 };
		int m_channels{ 0 };
		std::byte* m_data{ nullptr };
	};
}
