#ifndef PHIL_TEMPLATE_LIBRARY_UTILITY_CONSTEXPR_ALGORITHM_H
#define PHIL_TEMPLATE_LIBRARY_UTILITY_CONSTEXPR_ALGORITHM_H

#include <algorithm>
#include <iterator>

namespace ptl {
namespace constexpr_algorithm
{
	
namespace detail
{
	template <typename ITER_T>
	constexpr auto distance(ITER_T first, ITER_T last, ::std::random_access_iterator_tag)
	{
		return last-first;
	}
	
	template <typename ITER_T>
	constexpr auto distance(ITER_T first, ITER_T last, ::std::forward_iterator_tag)
	{
		typename ::std::iterator_traits<ITER_T>::difference_type d{0};
		while(first++!=last) ++d;
		return d;
	}
}
	
template <typename ITER_T>
constexpr auto distance(ITER_T first, ITER_T last)
{
	return detail::distance(std::move(first),::std::move(last),typename ::std::iterator_traits<ITER_T>::iterator_category{});
}

template <typename T>
constexpr auto swap(T& first, T& second)
{
	T tmp=::std::move(first);
	first=::std::move(second);
	second=::std::move(tmp);
}

template <typename T, typename U>
constexpr auto swap(::std::pair<T,U> &first, ::std::pair<T,U> &second)
{
	ptl::constexpr_algorithm::swap(first.first,second.first);
	ptl::constexpr_algorithm::swap(first.second,second.second);
}

namespace detail
{
	template <typename ITER_T>
	constexpr ITER_T left_child(ITER_T first, ITER_T last, ITER_T root)
	{
		auto id=2*::ptl::constexpr_algorithm::distance(first,root)+1;
		return id<::ptl::constexpr_algorithm::distance(first,last)?first+id:last;
	}
	
	template <typename ITER_T>
	constexpr ITER_T right_child(ITER_T first, ITER_T last, ITER_T root)
	{
		auto id=2*::ptl::constexpr_algorithm::distance(first,root)+2;
		return id<::ptl::constexpr_algorithm::distance(first,last)?first+id:last;
	}
	
	
	template <typename ITER_T>
	constexpr ITER_T heap_parent(ITER_T first, ITER_T last, ITER_T child)
	{
		return first+::ptl::constexpr_algorithm::distance(first,child)/2;
	}
	
	template <typename ITER_T, typename COMPARE_T>
	constexpr void siftdown_heap(ITER_T first, ITER_T last, ITER_T root, COMPARE_T compare)
	{
		auto left=left_child(first,last,root);
		auto right=right_child(first,last,root);
		while(left<last)
		{
			auto swap_with=root;
			if(compare(*swap_with,*left))
				swap_with=left;
			if(right!=last && compare(*swap_with,*right))
				swap_with=right;
			
			if(swap_with!=root)
				ptl::constexpr_algorithm::swap(*swap_with,*root);
			else
				return;
				
			root=swap_with;
			left=left_child(first,last,root);
			right=right_child(first,last,root);
		}
	}
}

template <typename ITER_T, typename COMPARE_T>
constexpr void make_heap(ITER_T first, ITER_T last, COMPARE_T compare)
{
	static_assert(::std::is_convertible<typename ::std::iterator_traits<ITER_T>::iterator_category, ::std::random_access_iterator_tag>::value,"make_heap requires random access iterators ;_;");
	
	auto root=detail::heap_parent(first,last,last-1);
	for(;;)
	{
		detail::siftdown_heap(first,last, root, compare);
		
		if(root==first)
			break;
		--root;
	}
}

template <typename ITER_T>
constexpr void make_heap(ITER_T first, ITER_T last) { ::ptl::constexpr_algorithm::make_heap(first,last,::std::less<>{}); }

template <typename ITER_T, typename COMPARE_T>
constexpr void pop_heap(ITER_T first, ITER_T last, COMPARE_T compare)
{
	--last;
	::ptl::constexpr_algorithm::swap(*first,*last);
	detail::siftdown_heap(first,last, first, compare);
}

template <typename ITER_T, typename COMPARE_T>
constexpr void sort_heap(ITER_T first, ITER_T last, COMPARE_T compare)
{
	while(first!=last)
	{
		::ptl::constexpr_algorithm::pop_heap(first,last,compare);
		--last;
	}
}

template <typename ITER_T>
constexpr void sort_heap(ITER_T first, ITER_T last) { ::ptl::constexpr_algorithm::sort_heap(first,last,::std::less<>{}); }


template <typename ITER_T, typename COMPARE_T>
constexpr void sort(ITER_T first, ITER_T last, COMPARE_T compare)
{
	//heapsort because im lazy... xD
	::ptl::constexpr_algorithm::make_heap(first,last,compare);
	::ptl::constexpr_algorithm::sort_heap(first,last,compare);
}

template <typename ITER_T>
constexpr void sort(ITER_T first, ITER_T last) { ::ptl::constexpr_algorithm::sort(first,last,::std::less<>{}); }

}}

#endif
