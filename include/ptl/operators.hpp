#ifndef PHIL_TEMPLATE_LIBRARY_OPERATORS_H
#define PHIL_TEMPLATE_LIBRARY_OPERATORS_H

namespace ptl {
namespace operators
{
	template <typename CHILD_T>
	struct bitwise_or
	{
		friend constexpr auto operator|(CHILD_T lhs, const CHILD_T& rhs) noexcept(noexcept(lhs|=rhs))
		{
			lhs|=rhs;
			return lhs;
		}
	};
	
	template <typename CHILD_T>
	struct bitwise_and
	{
		friend constexpr auto operator&(CHILD_T lhs, const CHILD_T& rhs) noexcept(noexcept(lhs&=rhs))
		{
			lhs&=rhs;
			return lhs;
		}
	};
	
	template <typename CHILD_T>
	struct bitwise_xor
	{
		friend auto constexpr operator^(CHILD_T lhs, const CHILD_T& rhs) noexcept(noexcept(lhs^=rhs))
		{
			lhs^=rhs;
			return lhs;
		}
	};
	
	template <typename CHILD_T>
	struct bitwise: bitwise_or<CHILD_T>, bitwise_and<CHILD_T>, bitwise_xor<CHILD_T> {};
	
	template <typename CHILD_T>
	struct plus
	{
		friend constexpr auto operator+(CHILD_T lhs, const CHILD_T& rhs) noexcept(noexcept(lhs+=rhs))
		{
			lhs+=rhs;
			return lhs;
		}
	};
	
	template <typename CHILD_T>
	struct minus
	{
		friend constexpr auto operator-(CHILD_T lhs, const CHILD_T& rhs) noexcept(noexcept(lhs-=rhs))
		{
			lhs-=rhs;
			return lhs;
		}
	};
	
	template <typename CHILD_T>
	struct multiply
	{
		friend auto constexpr operator*(CHILD_T lhs, const CHILD_T& rhs) noexcept(noexcept(lhs*=rhs))
		{
			lhs*=rhs;
			return lhs;
		}
	};
	
	template <typename CHILD_T>
	struct divide
	{
		friend auto constexpr operator/(CHILD_T lhs, const CHILD_T& rhs) noexcept(noexcept(lhs/=rhs))
		{
			lhs/=rhs;
			return lhs;
		}
	};
	
	template <typename CHILD_T>
	struct arithmetic: plus<CHILD_T>, minus<CHILD_T>, multiply<CHILD_T>, divide<CHILD_T> {};
	
}} //end namespace ptl::operators

#endif
