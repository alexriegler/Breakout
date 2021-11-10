module;

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)

export module stb_image;

namespace stb {
	// Renames and exports functions
	export constexpr auto& set_flip_vertically_on_load = stbi_set_flip_vertically_on_load;
	export constexpr auto& load = stbi_load;
	export constexpr auto& image_free = stbi_image_free;
}