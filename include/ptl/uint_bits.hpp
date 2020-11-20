#ifndef PHIL_TEMPLATE_LIBRARY_UINT_BITS_H
#define PHIL_TEMPLATE_LIBRARY_UINT_BITS_H

#include <type_traits>

#include <cstddef>
#include <cstdint>

namespace ptl
{

namespace detail
{
	constexpr std::size_t next_power_of_two(std::size_t i, std::size_t last=1)
	{
		return i<=last?last:next_power_of_two(i,last<<1);
	}

	constexpr std::size_t log2integer(std::size_t i)
	{
		std::size_t count=0;
		while(i>1)
		{
			++count;
			i>>=1;
		}
		return count;
	}
}
	
template <std::size_t BITS>
struct uint_bits;
	
template <> struct uint_bits<8> { using type=std::uint8_t; };
template <> struct uint_bits<16> { using type=std::uint16_t; };
template <> struct uint_bits<32> { using type=std::uint32_t; };
template <> struct uint_bits<64> { using type=std::uint64_t; };
	
template <std::size_t BITS> using uint_bits_t=typename uint_bits<BITS>::type;
	
template <std::size_t BITS, typename=std::void_t<>>
struct uint_least_bits
{
	using type=typename uint_least_bits<detail::next_power_of_two(BITS+1)>::type;
};
	
template <std::size_t BITS>
struct uint_least_bits<BITS,std::void_t<typename uint_bits<BITS>::type> >
{
	using type=typename uint_bits<BITS>::type;
};
	
template <std::size_t BITS> using uint_least_bits_t=typename uint_least_bits<BITS>::type;

template <std::size_t MAX_VALUE> using uint_for_t=typename uint_least_bits<detail::log2integer(detail::next_power_of_two(MAX_VALUE+1))>::type;

} //end namespace ptl

#endif
