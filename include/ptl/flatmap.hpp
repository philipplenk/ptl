#ifndef PHIL_TEMPLATE_LIBRARY_FLATMAP_H
#define PHIL_TEMPLATE_LIBRARY_FLATMAP_H

#include <ptl/constexpr_algorithm.hpp>
#include <ptl/ebo.hpp>

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace ptl
{
	template <typename KEY_T, typename MAPPED_T, typename COMPARE_T=std::less<KEY_T>, typename CONTAINER_T=std::vector<std::pair<KEY_T,MAPPED_T>>>
	class flatmap
	{
		public:
		using key_type					=	KEY_T;
		using mapped_type				=	MAPPED_T;
		using value_type				=	std::pair<KEY_T,MAPPED_T>;
		using size_type					=	typename CONTAINER_T::size_type;
		using difference_type			=	typename CONTAINER_T::difference_type;
		using key_compare				=	COMPARE_T;
		
		using reference					=	value_type&;
		using const_reference			=	const value_type&;
		
		using pointer					=	typename CONTAINER_T::pointer;
		using const_pointer				=	typename CONTAINER_T::const_pointer;
		
		using iterator					=	typename CONTAINER_T::iterator;
		using const_iterator			=	typename CONTAINER_T::const_iterator;
		using reverse_iterator			=	typename CONTAINER_T::reverse_iterator;
		using const_reverse_iterator	=	typename CONTAINER_T::const_reverse_iterator;
		
		struct value_compare_t: ptl::ebo_base<COMPARE_T>
		{
			template <typename... T>
			constexpr value_compare_t(T&& ...args):
				ptl::ebo_base<COMPARE_T>(std::forward<T>(args)...)
			{}
			
			constexpr auto get() const noexcept { return this->get_ebo_base(ptl::typelist<COMPARE_T>{}); }
			
			constexpr bool operator()(const value_type& lhs, const value_type &rhs) const
			{
				return get().operator()(lhs.first,rhs.first);
			}
			
			template <typename T>
			constexpr bool operator()(const value_type &lhs, const T& rhs) const
			{
				return get().operator()(lhs.first,rhs);
			}
			
			template <typename T>
			constexpr bool equal(const value_type &lhs, const T& rhs) const
			{
				return !get().operator()(lhs.first,rhs) && !get().operator()(rhs,lhs.first);
			}
			
			constexpr bool equal(const value_type &lhs, const value_type &rhs) const
			{
				return !operator()(lhs,rhs) && !operator()(rhs,lhs);
			}
		};
		
		constexpr flatmap():
			flatmap(COMPARE_T{})
		{}
		
		template <typename ...ARG_T>
		explicit constexpr flatmap(const COMPARE_T &comp, ARG_T&& ...container_args):
			data_{comp,std::forward<ARG_T>(container_args)...}
		{}
		
		constexpr auto begin() noexcept { return data_.storage.begin(); }
		constexpr auto begin() const noexcept { return data_.storage.begin(); }
		constexpr auto end() noexcept { return data_.storage.end(); }
		constexpr auto end() const noexcept { return data_.storage.end(); }
		
		constexpr auto rbegin() noexcept { return data_.storage.rbegin(); }
		constexpr auto rbegin() const noexcept { return data_.storage.rbegin(); }
		constexpr auto rend() noexcept { return data_.storage.rend(); }
		constexpr auto rend() const noexcept { return data_.storage.rend(); }
		
		constexpr auto cbegin() const noexcept { return begin(); }
		constexpr auto cend() const noexcept { return end(); }
		constexpr auto crbegin() const noexcept { return rbegin(); }
		constexpr auto crend() const noexcept { return rend(); }
		
		constexpr auto empty() const noexcept { return data_.storage.empty(); }
		constexpr auto size() const noexcept { return data_.storage.size(); }
		
		constexpr auto find(const key_type& key)
		{
			return find_impl(*this,key);
		}
		
		constexpr auto find(const key_type& key) const
		{
			return find_impl(*this,key);
		}
		
		constexpr std::pair<iterator, bool> insert(const value_type &new_val)
		{
			auto it=std::lower_bound(std::begin(data_.storage),std::end(data_.storage),new_val,value_compare());
			if(it!=data_.storage.end() && value_compare().equal(*it,new_val))
				return std::pair<iterator, bool>(it,false);
			return std::pair<iterator, bool>(data_.storage.insert(it,new_val),true);
		}
		
		constexpr auto erase(iterator pos) { return data_.storage.erase(pos); }
		constexpr auto erase(const_iterator pos) { return data_.storage.erase(pos); }
		
		
		constexpr size_type erase(const key_type& key)
		{
			if(auto it=find(key); it!=data_.storage.end())
			{
				erase(it);
				return 1;
			}
			return 0;
		}
		
		constexpr auto& operator[](const key_type &key)
		{
			auto it=std::lower_bound(std::begin(data_.storage),std::end(data_.storage),key,value_compare());
			if(it!=data_.storage.end() && value_compare().equal(*it,key))
				return it->second;
			return data_.storage.insert(it,value_type(key,{}))->second;
		}
		
		constexpr const auto& operator[](const key_type &key) const
		{
			auto it=std::lower_bound(std::begin(data_.storage),std::end(data_.storage),key,value_compare());
			if(it!=data_.storage.end() && value_compare().equal(*it,key))
				return it->second;
			
			throw std::out_of_range{"Tried to access nonexistent element with operator[] on a const object..."};
		}
		
		private:
		struct member_data: ebo_base<value_compare_t>
		{
			public:
			template <typename ...ARG_T>
			constexpr member_data(const value_compare_t& comp, ARG_T&& ...args):
				ebo_base<value_compare_t>{comp},
				storage(std::forward<ARG_T>(args)...)
			{}
			
			CONTAINER_T storage;
		} data_;
		
		constexpr const auto& value_compare() const noexcept { return data_.get_ebo_base(typelist<value_compare_t>{}); }
		
		template <typename const_deduced_self>
		constexpr static auto find_impl(const_deduced_self& self, const key_type& key)
		{
			auto it=std::lower_bound(std::begin(self.data_.storage),std::end(self.data_.storage),key,self.value_compare());
			if(it==self.data_.storage.end() || !self.value_compare().equal(*it,key))
				return self.data_.storage.end();
			return it;
		}
	};
	
	template <typename KEY_T, typename MAPPED_T, std::size_t NUM, typename COMPARE_T=std::less<KEY_T>, typename CONTAINER_T=std::array<std::pair<KEY_T,MAPPED_T>,NUM>>
	using fixed_flatmap=flatmap<KEY_T,MAPPED_T,COMPARE_T,CONTAINER_T>;
	
	namespace detail
	{	
		template <typename KEY_T, typename MAPPED_T, std::size_t NUM, typename COMPARE_T=std::less<KEY_T>, typename CONTAINER_T=std::array<std::pair<KEY_T,MAPPED_T>,NUM>, std::size_t ...IDX>
		constexpr auto make_fixed_flatmap(COMPARE_T compare, const std::pair<KEY_T,MAPPED_T> (&elements)[NUM],std::index_sequence<IDX...>)
		{
			std::array<std::pair<KEY_T,MAPPED_T>,NUM> arr{{elements[IDX]...}};
			
			const auto compare_first = [&](const auto& lhs, const auto& rhs)
			{
				return compare(lhs.first,rhs.first);
			};
			
			ptl::constexpr_algorithm::sort(std::begin(arr),std::end(arr),compare_first);
			return ptl::flatmap<KEY_T,MAPPED_T,COMPARE_T,CONTAINER_T>{compare,arr};
		}
	}
	
	template <typename KEY_T, typename MAPPED_T, std::size_t NUM, typename COMPARE_T=std::less<KEY_T>, typename CONTAINER_T=std::array<std::pair<KEY_T,MAPPED_T>,NUM>>
	constexpr auto make_fixed_flatmap(COMPARE_T compare, const std::pair<KEY_T,MAPPED_T> (&elements)[NUM])
	{
		return detail::make_fixed_flatmap<KEY_T,MAPPED_T,NUM,COMPARE_T,CONTAINER_T>(compare,elements,std::make_index_sequence<NUM>{});
	}
	
	template <typename KEY_T, typename MAPPED_T, std::size_t NUM, typename CONTAINER_T=std::array<std::pair<KEY_T,MAPPED_T>,NUM>>
	constexpr auto make_fixed_flatmap(const std::pair<KEY_T,MAPPED_T> (&elements)[NUM])
	{
		return detail::make_fixed_flatmap<KEY_T,MAPPED_T,NUM,std::less<KEY_T>,CONTAINER_T>(std::less<KEY_T>{},elements,std::make_index_sequence<NUM>{});
	}
	
} //end namespace ptl

#endif
