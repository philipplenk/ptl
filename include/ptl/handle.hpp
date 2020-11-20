#ifndef PHIL_TEMPLATE_LIBRARY_HANDLE_H
#define PHIL_TEMPLATE_LIBRARY_HANDLE_H

#include <functional>
#include <type_traits>

#include <cstddef>

namespace ptl
{
	template <typename TAG_T, typename INTERNAL_T=std::size_t>
	class handle
	{
		public:
		template <typename TAG_TMP=TAG_T, typename = std::void_t<decltype(TAG_TMP::default_value)>>
		explicit constexpr handle() noexcept(noexcept(handle{TAG_T::default_value})):
			handle{TAG_T::default_value}
		{}
		
		explicit constexpr handle(INTERNAL_T value) noexcept(std::is_nothrow_copy_constructible<INTERNAL_T>::value):
			value(value)
		{}
		
		explicit constexpr operator INTERNAL_T() const noexcept(std::is_nothrow_copy_constructible<INTERNAL_T>::value)
		{
			return value;
		}
		
		constexpr INTERNAL_T underlying() const noexcept(std::is_nothrow_copy_constructible<INTERNAL_T>::value)
		{
			return value;
		}
		
		friend constexpr bool operator==(const handle &lhs, const handle &rhs) noexcept(noexcept(lhs.value==rhs.value))
		{
			return lhs.value==rhs.value;
		}
		
		friend constexpr bool operator!=(const handle &lhs, const handle &rhs) noexcept(noexcept(!(lhs==rhs)))
		{
			return !(lhs==rhs);
		}
		
		friend constexpr bool operator<(const handle &lhs, const handle &rhs) noexcept(noexcept(lhs.value<rhs.value))
		{
			return lhs.value<rhs.value;
		}
		
		friend constexpr bool operator<=(const handle &lhs, const handle &rhs) noexcept(noexcept(lhs.value<=rhs.value))
		{
			return lhs.value<=rhs.value;
		}
		
		friend constexpr bool operator>(const handle &lhs, const handle &rhs) noexcept(noexcept(lhs.value>rhs.value))
		{
			return lhs.value>rhs.value;
		}
		
		friend constexpr bool operator>=(const handle &lhs, const handle &rhs) noexcept(noexcept(lhs.value>=rhs.value))
		{
			return lhs.value>=rhs.value;
		}
		
		private:
		INTERNAL_T value;
	};
	
	template <typename T>
	struct underlying_type;
	
	template <typename T, typename U>
	struct underlying_type<handle<T,U>>
	{
		using type=U;
	};
}

//yeahy, legally modifying std namespace ;-);
namespace std
{
	template <typename TAG_T, typename INTERNAL_T>
	struct hash<::ptl::handle<TAG_T,INTERNAL_T>>
	{
		constexpr size_t operator()(const ::ptl::handle<TAG_T,INTERNAL_T>& v) const
		{
			return std::hash<INTERNAL_T>()(static_cast<INTERNAL_T>(v));
		}
	};
}

#endif
