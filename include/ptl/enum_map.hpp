#ifndef PHIL_TEMPLATE_LIBRARY_ENUM_MAP_H
#define PHIL_TEMPLATE_LIBRARY_ENUM_MAP_H

#include <array>
#include <type_traits>

namespace ptl
{
	template <typename enum_type, typename mapped_type, std::size_t number_of_values>
	class enum_map 
	{
		public:
		using underlying=std::underlying_type_t<enum_type>;
		
		constexpr auto& operator[](enum_type v) noexcept { return data[static_cast<underlying>(v)]; }
		constexpr const auto& operator[](enum_type v) const noexcept { return data[static_cast<underlying>(v)]; }
		
		constexpr auto begin() noexcept { return data.begin(); }
		constexpr auto end() noexcept { return data.end(); }
		
		constexpr auto begin() const noexcept { return data.begin(); }
		constexpr auto end() const noexcept { return data.end(); }
		
		constexpr auto cbegin() const noexcept { return data.cbegin(); }
		constexpr auto cend() const noexcept { return data.cend(); }
		
		constexpr auto size() const noexcept { return number_of_values; }
		
		std::array<mapped_type,number_of_values> data{};
	};
	
} //end namespace ptl;

#endif
