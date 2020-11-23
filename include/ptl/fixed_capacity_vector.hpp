#ifndef PHIL_TEMPLATE_LIBRARY_FIXED_CAPACITY_VECTOR_H
#define PHIL_TEMPLATE_LIBRARY_FIXED_CAPACITY_VECTOR_H

#include <ptl/uint_bits.hpp>

#include <algorithm>
#include <array>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include <cstddef>

namespace ptl
{
	namespace detail
	{
		template <typename T, std::size_t CAPACITY>
		struct fixed_capacity_storage_literal
		{
			constexpr void clear() noexcept { used=0; }
			
			constexpr void construct_at(ptl::uint_for_t<CAPACITY> pos, const T& val) noexcept(std::is_nothrow_copy_assignable_v<T>)
			{
				data[pos] = val;
			}
			
			constexpr void construct_at(ptl::uint_for_t<CAPACITY> pos, T&& val) noexcept(std::is_nothrow_move_assignable_v<T>)
			{
				data[pos] = std::move(val);
			}
			
			constexpr void destroy_at(ptl::uint_for_t<CAPACITY> pos) noexcept {}
			
			constexpr T* data_ptr() noexcept { return &data[0]; }
			constexpr const T* data_ptr() const noexcept { return &data[0]; }
			
			std::array<T,CAPACITY> data{};
			ptl::uint_for_t<CAPACITY> used=0;
		};
		
		template <typename T, std::size_t CAPACITY>
		struct fixed_capacity_storage
		{
			fixed_capacity_storage() noexcept = default;
			
			~fixed_capacity_storage() noexcept
			{
				std::destroy_n(data_ptr(),used);
			}
			
			fixed_capacity_storage(const fixed_capacity_storage& other) noexcept(std::is_nothrow_copy_constructible_v<T>):
				used{other.used}
			{
				std::uninitialized_copy_n(other.data_ptr(),used,data_ptr());
			}
			
			fixed_capacity_storage(fixed_capacity_storage&& other) noexcept(std::is_nothrow_move_constructible_v<T>):
				used{other.used}
			{
				std::uninitialized_move_n(other.data_ptr(),used,data_ptr());
			}
			
			//making the following two exception safe is a bit of a pain and also somewhat expensive at runtime.
			//As such, I sacrifice some guarantees here. If copying/moving fails, we are simply left in an emtpy state.
			fixed_capacity_storage& operator=(const fixed_capacity_storage& other) noexcept(std::is_nothrow_copy_constructible_v<T>)
			{
				clear();
				used = other.used;
				std::uninitialized_copy_n(other.data_ptr(),used,data_ptr());
				
				return *this;
			}
			
			fixed_capacity_storage& operator=(fixed_capacity_storage&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
			{
				clear();
				used = other.used;
				std::uninitialized_move_n(other.data_ptr(),used,data_ptr());
				
				return *this;
			}
			
			void clear() noexcept
			{
				std::destroy_n(data_ptr(),used);
				used=0;
			}
			
			void construct_at(ptl::uint_for_t<CAPACITY> pos, const T& val) noexcept(std::is_nothrow_copy_constructible_v<T>)
			{
				::new(&data_ptr()[pos]) T{val};
			}
			
			void construct_at(ptl::uint_for_t<CAPACITY> pos, T&& val) noexcept(std::is_nothrow_copy_constructible_v<T>)
			{
				::new(&data_ptr()[pos]) T{::std::move(val)};
			}
			
			void destroy_at(ptl::uint_for_t<CAPACITY> pos) noexcept(std::is_nothrow_destructible_v<T>)
			{
				data_ptr()[pos].~T();
			}
			
			T* data_ptr() noexcept { return static_cast<T*>(static_cast<void*>(data)); }
			const T* data_ptr() const noexcept { return static_cast<const T*>(static_cast<const void*>(data)); }
			
			std::aligned_storage_t<sizeof(T),alignof(T)> data[CAPACITY]={};
			
			ptl::uint_for_t<CAPACITY> used=0;
		};
	}
	
	template <typename T, std::size_t CAPACITY>
	class fixed_capacity_vector
	{
		//In all my other class(template) declarations, I prefer to have private(and data members in general) below public functions
		//Putting it first is a simple workaround for this gcc bug: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=52869 preventing noexcept from seeing member variables
		//Its fixed in gcc 10, but fails to compile with older versions
		private:
		using storage_t=std::conditional_t
		<
			std::is_trivially_default_constructible_v<T> && std::is_trivially_destructible_v<T>,
			detail::fixed_capacity_storage_literal<T,CAPACITY>,
			detail::fixed_capacity_storage<T,CAPACITY>
		>;
		
		storage_t storage_;
		
		public:
		using value_type		=	T;
		using reference			=	value_type&;
		using const_reference	=	const value_type&;
		using pointer			=	T*;
		using const_pointer		=	const T*;
		using size_type			=	ptl::uint_for_t<CAPACITY>;
		
		constexpr auto size() const noexcept { return storage_.used; }
		constexpr auto empty() const noexcept { return size()==0; }
		constexpr auto capacity() const noexcept { return CAPACITY; }
		constexpr auto max_size() const noexcept { return capacity(); }
		
		constexpr reference operator[](size_type id) noexcept { return data()[id]; }
		constexpr const_reference operator[](size_type id) const noexcept { return data()[id]; }

		#if __cpp_exceptions		
		constexpr reference at(size_type id) { if(!(id<size())) throw std::out_of_range{"Accessed fixed_capacity_vector out of range ;_;"}; return data()[id]; }
		constexpr const_reference at(size_type id) const { if(!(id<size())) throw std::out_of_range{"Accessed fixed_capacity_vector out of range ;_;"}; return data()[id]; }
		#endif
		
		constexpr reference front() noexcept { return data()[0]; }
		constexpr const_reference front() const noexcept { return data()[0]; }
		
		constexpr reference back() noexcept { return data()[size()-1]; }
		constexpr const_reference back() const noexcept { return data()[size()-1]; }
		
		constexpr pointer data() noexcept {  return storage_.data_ptr(); }
		constexpr const_pointer data() const noexcept { return storage_.data_ptr(); }
		
		constexpr auto begin() noexcept { return data(); }
		constexpr auto begin() const noexcept { return data(); }
		constexpr auto cbegin() const noexcept { return data(); }
		
		constexpr auto rbegin() noexcept { return std::make_reverse_iterator(end()); }
		constexpr auto rbegin() const noexcept { return std::make_reverse_iterator(end()); }
		constexpr auto crbegin() const noexcept { return std::make_reverse_iterator(end()); }
		
		constexpr auto end() noexcept { return (&data()[size()-1])+1; }
		constexpr auto end() const noexcept { return (&data()[size()-1])+1; }
		constexpr auto cend() const noexcept { return (&data()[size()-1])+1; }
		
		constexpr auto rend() noexcept { return std::make_reverse_iterator(begin()); }
		constexpr auto rend() const noexcept { return std::make_reverse_iterator(begin()); }
		constexpr auto crend() const noexcept { return std::make_reverse_iterator(begin()); }
		
		constexpr void clear() noexcept(noexcept(storage_.clear())) { storage_.clear(); }
		
		constexpr void push_back(const_reference value) noexcept(noexcept(storage_.construct_at(size(),value))) { storage_.construct_at(size(),value); ++storage_.used;  }
		constexpr void push_back(T&& value) noexcept(noexcept(storage_.construct_at(size(),std::move(value)))) { storage_.construct_at(size(),std::move(value)); ++storage_.used; }
		
		constexpr void pop_back() noexcept(std::is_nothrow_destructible<T>::value) { --storage_.used; storage_.destroy_at(size()); }
	};
}

#endif
