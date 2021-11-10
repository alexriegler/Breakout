#pragma once

#include <concepts>
#include <type_traits>

namespace ar
{
	template <typename T>
	concept Enumeration = std::is_enum_v<T>;

	/// <summary>
	/// Returns the underlying value associated with an enumeration.
	/// </summary>
	/// <param name="e">The enum value</param>
	/// <returns>The underlying value</returns>
	template <Enumeration Enum>
	constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
	{
		return static_cast<std::underlying_type_t<Enum>>(e);
	}
}