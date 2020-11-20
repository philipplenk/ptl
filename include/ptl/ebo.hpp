#ifndef PHIL_TEMPLATE_LIBRARY_EBO_H
#define PHIL_TEMPLATE_LIBRARY_EBO_H

#include <ptl/typelist.hpp>

#include <type_traits>
#include <utility>

/***
  Very simple and suboptimal helper for potentially empty classes. Will become entirely useless and unnecessary with C++20's [[no_unique_address]]
  Simply inherit from ebo_bases<your,maybe,emtpy,bases> and it will deal with stuff that cannot be inherited from directly
  To access any of the objects call get_ebo_base(typelist<classname>). If classname is not itself a template parameter of your class and you are within the inheriting class,
  the classname has to be fully qualified to disambiguate it from the injected one.
***/

namespace ptl
{
	template <typename T, typename = void>
	class ebo_base
	{
		public:
		template <typename ...ARGS_T>
		constexpr ebo_base(ARGS_T&& ...args):
			value(std::forward<ARGS_T>(args)...)
		{}
		
		T& get_ebo_base(typelist<T>) { return value; }
		const T& get_ebo_base(typelist<T>) const { return value; }
		
		private:
		T value;
	};
	
	template <typename T>
	class ebo_base<T,
		std::enable_if_t<std::is_class<T>::value && std::is_empty<T>::value && !std::is_final<T>::value>>: private T
	{
		public:
		template <typename... ARGS_T>
		constexpr ebo_base(ARGS_T&& ...args):
			T(std::forward<ARGS_T>(args)...)
		{}
		
		T& get_ebo_base(typelist<T>) { return *this; }
		const T& get_ebo_base(typelist<T>) const { return *this; }
	};
	
	template <bool default_constructible, typename...> class ebo_bases_impl;
	
	template <typename... base_types>
	class ebo_bases_impl<true,base_types...>: public ebo_base<base_types>...
	{
		public:
		constexpr ebo_bases_impl() = default;
		
		constexpr ebo_bases_impl(ebo_base<base_types>... bases):
			ebo_base<base_types>{std::move(bases)}...
		{}
		
		using ebo_base<base_types>::get_ebo_base...;
	};
	
	template <typename... base_types>
	class ebo_bases_impl<false,base_types...>: public ebo_base<base_types>...
	{
		public:
		constexpr ebo_bases_impl(ebo_base<base_types>... bases):
			ebo_base<base_types>{std::move(bases)}...
		{}
		
		using ebo_base<base_types>::get_ebo_base...;
	};
	
	template <typename... base_types>
	using ebo_bases = ebo_bases_impl<std::conjunction_v<std::is_default_constructible<base_types>...>,base_types...>;
}

#endif
