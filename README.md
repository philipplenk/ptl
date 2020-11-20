# phil template library 

A small collection of small things reused in almost all of my C++ projects. Whenever I end up creating a reasonably generic class template or similar I deem generally useful I tend to extract it and put it in here. Occasionally, it also contains replications of standard library functionality which is either not available in the standard version I use or is not constexpr when I need it to be.

**Be aware that not everything in here is polished or of the highest quality and thoughtlessly using it in your own projects might not be advisable.**

Those of you who know me are aware that I am rather shy and also don't believe anyone except me will ever use this, which is why this public repository only contains that which is strictly neccesary for my other *public* projects to compile and work and I have omitted everything else, including tests and documentation. If, contrary to my expectations, this proves useful to anyone, feel free to message me and ask for the rest and I shall update the repository once I find the time and courage ;-)

At the time of writing, the following is available in this header-only library:

- [*bit.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/bit.hpp) - Implements part of what the [C++20 standard header bit](https://en.cppreference.com/w/cpp/header/bit) provides for use with C++17.
- [*constexpr_algorithm.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/constexpr_algorithm.hpp) - Mostly a pathetic implementation of [std::sort](https://en.cppreference.com/w/cpp/algorithm/sort) and various algorithms it depends on, which are much less efficient and probably more buggy than the real thing, but have the benefit of being constexpr in C++17(which the standard sort is only in C++>=20)
- [*constexpr_mersenne_twister.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/constexpr_mersenne_twister.hpp) - Exactly what it says on the tin, same as above, a worse, but constexpr version of what [the standard provides](https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine)
- [*ebo.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/ebo.hpp) - A simple template helper to work with the potential optimization of empty base classes. You simply inherit privately from ebo_bases<any,class,or,nonclass,you,like> and it deals with potential final classes or other cases you can't directly inherit from and provides a simple interface to get a simple reference to it. Bound to become obsolete soon, thanks to C++20's [no_unique_address](https://en.cppreference.com/w/cpp/language/attributes/no_unique_address).
- [*enum_map.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/enum_map.hpp) - Basically a simple std::array, but not indexed by arbitrary integers but only members of a given contiguous enum class. I wrote about one of its usages in a [blog article on gameboy emulation](https://codemetas.de/2020/06/22/klobigb_overview.html).
- [*fixed_capacity_vector.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/fixed_capacity_vector.hpp) - A contiguous container of dynamic size but fixed capacity. Likely should not exist and could have been solved with an appropriate allocator for [std::vector](https://en.cppreference.com/w/cpp/container/vector) instead. Depends on *uint_bits.hpp*
- [*flatmap.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/flatmap.hpp) - A really simple flatmap, i.e. a sorted array mirroring the interface of [std::map](https://en.cppreference.com/w/cpp/container/map). Has the additional advantage of being usable at compile time when instantiated with an array as its underlying storage. It depends on *ebo.hpp* and *constexpr_algorithm.hpp*
- [*handle.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/handle.hpp) - A simple opaque handle. Tagged on a user provided type and holding a std::size_t or arbitrary other value, it is useful to prevent accidental misuse when handing out some form of ID to users. It only provides overloads for comparisons and hash, whilst constructing, accessing or modifying the stored value requires explicit casts. 
- [*operators.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/operators.hpp) - Uses the famous [Barton–Nackman trick](https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick) to define the binary operator@ overloads in terms of their operator@= equivalent. Simply opt in for a class X by inheriting, for instance, from ptl::operators::arithmetic<X>
- [*typelist.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/typelist.hpp) - The simplest of helper templates. Here it is, in its entirety: template <typename... T> typelist{};
- [*uint_bits.hpp*](https://github.com/philipplenk/ptl/blob/main/include/ptl/uint_bits.hpp) - Simple template to easily get the best fitting fixed integer type for a given bit or value number. ptl::uint_for_t<30000> equals std::uint16_t for instance. 
