#ifndef PHIL_TEMPLATE_LIBRARY_CONSTEXPR_MERSENNE_TWISTER_H
#define PHIL_TEMPLATE_LIBRARY_CONSTEXPR_MERSENNE_TWISTER_H

#include <array>

#include <cstdint>

namespace ptl
{

namespace detail
{
	template <typename T, std::size_t w>
	constexpr T compute_wbitmask() noexcept
	{
		T ret_val{0};
		for(std::size_t i=0;i<w;++i)
			ret_val=(ret_val<<1)|1;
		return ret_val;
	}
}

template
	<
		typename T,
		std::size_t w, std::size_t n, std::size_t m, std::size_t r,
		T a, std::size_t u, T d, std::size_t s,
		T b, std::size_t t,
		T c, std::size_t l, T f
	>
class mersenne_twister
{
	public:
	constexpr explicit mersenne_twister(T seed) noexcept
	{
		auto statep=&std::get<0>(state);
		statep[0]=seed;
		for(std::size_t i=1;i<n;++i)
			statep[i]=(f*((statep[i-1]^(statep[i-1]>>(w-2))))+i)&wbitmask;

		twist();
	}
	
	constexpr explicit mersenne_twister(const ::std::array<T,n> &state) noexcept:
		state{state}
	{
		twist();
	}
	
	constexpr T operator()() noexcept
	{
		auto statep=&::std::get<0>(state);
		auto ret_val=statep[idx];
		if(++idx==n)
			twist();
			
		ret_val^=(ret_val>>u) & d;
		ret_val^=(ret_val<<s) & b;
		ret_val^=(ret_val<<t) & c;
		ret_val^=(ret_val>>l);
		
		return ret_val&wbitmask;
	}
	
	private:
	std::array<T,n> state{};
	std::size_t idx=0;
	static constexpr auto wbitmask=detail::compute_wbitmask<T,w>();
	static constexpr auto lower_mask=(1u<<r)-1;
	static constexpr auto upper_mask=(~lower_mask)&wbitmask;
	
	constexpr void twist() noexcept
	{
		auto statep=&::std::get<0>(state);
		for(std::size_t i=0;i<n;++i)
		{
			auto tmp=(statep[i] & upper_mask) + (statep[(i+1)%n] & lower_mask);
			auto tmp_a=tmp>>1;
			if(tmp%2!=0)
				tmp_a^=a;
			statep[i]=statep[(i+m)%n] ^ tmp_a;
		}
		idx=0;
	}
};

using mersenne_twister19937=mersenne_twister<std::uint32_t, 32, 624, 397, 31, 0x9908b0df, 11, 0xffffffff, 7, 0x9d2c5680, 15, 0xefc60000, 18, 1812433253>;
using mersenne_twister19937_64=mersenne_twister<std::uint64_t, 64, 312, 156, 31, 0xb5026f5aa96619e9, 29, 0x5555555555555555, 17, 0x71d67fffeda60000, 37, 0xfff7eee000000000, 43, 6364136223846793005>;

} //end namespace ptl

#endif
