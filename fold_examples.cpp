#include <array>
#include <tuple>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "cpp_17.h"
#include "any_fold.h"
#include <type_traits>


// 1. -------------------------------------------------------------------------
struct Printer
{
	template <class T> void operator()(T&& arg) { std::cout << arg; }
};
// 2. -------------------------------------------------------------------------
namespace detail
{
	template <class... Ts>
	constexpr auto sum_(Ts&&... args)
	{
		return (args + ...);
	}

	template <typename T, std::size_t N, std::size_t... Is>
	constexpr T sum_impl(std::array<T, N> const &arr, std::index_sequence<Is...>)
	{
		return sum_(arr[Is]...);
	}
}

template <typename T, std::size_t N>
constexpr T sum(std::array<T, N> const &arr)
{
	return detail::sum_impl(arr, std::make_index_sequence<N>{});
}
// 3. -------------------------------------------------------------------------
struct Summer
{
	template <class... Ts>
	constexpr auto operator()(Ts&&... args) { return (args + ...); }
};
// 4. -------------------------------------------------------------------------
template <class T>
std::string Stringify(T const& value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

struct Join
{
	template <class T1, class T2>
	std::string operator()(T1 const& lhs, T2 const& rhs)
	{
		return Stringify(lhs) + Stringify(rhs);
	}
};

struct Vettore
{
	std::vector<int> operator()(int a, int b)
	{
		return{ a, b };
	}

	std::vector<int> operator()(int b, std::vector<int> const& a)
	{
		auto ret(a);
		ret.insert(ret.begin(), b);
		return ret;
	}
};

struct Max
{
	template <class T1, class T2>
	constexpr decltype(auto) operator()(T1&& lhs, T2&& rhs)
	{
		return lhs > rhs ? std::forward<T1>(lhs) : std::forward<T2>(rhs);
	}
};
// ~ --------------------------------------------------------------------------

void fold_examples()
{
	// 1. A for each lambda ---------------------------------------------------
	auto ForEach = [](auto&& fun, auto&&... args) {
		(fun(args), ...);
	};
	ForEach(Printer{}, 1, " to the ", 2, " to the ", 3, '\n');
	// 2. Sum the contents of an std::array at compile time -------------------
	constexpr std::array<int, 4> arr{ { 1, 1, 2, 3 } };
	std::cout << "Array sum : " << std::integral_constant<int, sum(arr)>{} << std::endl;
	// 3. Sum the contents of a tuple at compile time -------------------------
	constexpr std::tuple<int, int, int, int> tup{ 1, 1, 2, 3 };
	std::cout << "Tuple sum : " << std::integral_constant<int,
		cpp17::apply(Summer{}, tup)>{} << std::endl;
	// 4. Use fold expressions for arbitrary operators ------------------------
	std::cout << fld::foldr<Join>(1, std::string(
		" bird in the hand, is worth "), 10, std::string(" in the bush")) << std::endl;

	auto k = fld::foldl<Vettore>(1, 2, 30, 12);
	for (auto&& i : k) std::cout << i << std::endl;

	static_assert(20 == fld::foldl<Max>(1, 20, 3, 5), "ET phone home");
	std::cout << "Reducing the maximum of [1, 20, 3, 5] : "
		<< fld::foldl<Max>(1, 20, 3, 5) << std::endl;
}



