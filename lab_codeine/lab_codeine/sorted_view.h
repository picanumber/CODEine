#ifndef SORTED_VIEW_HPP_04012017
#define SORTED_VIEW_HPP_04012017

#include <tuple>
#include <vector>
#include <numeric>
#include <algorithm>

namespace utl
{
	namespace detail
	{
		using std::get; 
		using std::size_t; 
		using std::forward; 

		// 0: Equal, 1: lhs < rhs, -1: lhs > rhs
		template <class F, class T, class U>
		constexpr int Compare(F&& less, T&& lhs, U&& rhs)
		{ 
			if (forward<F>(less)(forward<T>(lhs), forward<U>(rhs)))
			{
				return 1; 
			}
			else if (forward<F>(less)(forward<U>(rhs), forward<T>(lhs)))
			{
				return -1; 
			}
			return 0; // when none is less than the other, they're equal
		}

		template <size_t... Is> struct use_comparator; 

		template <size_t I, std::size_t... Is>
		struct use_comparator<I, Is...>
		{
			template <class Tuple, class F>
			constexpr static bool apply(Tuple const& t, size_t indexL, size_t indexR, F&& bop)
			{
				switch (Compare(forward<F>(bop), get<I>(t)[indexL], get<I>(t)[indexR]))
				{
				case  1: return true; 
				case -1: return false; 
				default: return use_comparator<Is...>::apply(t, indexL, indexR, forward<F>(bop));
				}
			}
		};

		template <>
		struct use_comparator<>
		{
			template <class Tuple, class F>
			constexpr static bool apply(Tuple const&, size_t, size_t, F&&) { return false; }
		}; 
	}

	template <class... Args>
	class sorted_view
	{
		std::tuple<Args...>      _ts;
		mutable std::vector<std::size_t> _is;

	public:
		using tuple_t = decltype(_ts);

		sorted_view(Args&&... args) 
			: _ts(std::forward<Args>(args)...)
			, _is(std::get<0>(_ts).size())
		{ }

		template <std::size_t I, class F>
		void sort(F&& bop) const
		{
			using namespace std;

			_is.resize(get<I>(_ts).size());
			iota(begin(_is), end(_is), 0);

			std::sort(begin(_is), end(_is), [&](size_t lhs, size_t rhs) {
				return forward<F>(bop)(get<I>(_ts)[lhs], get<I>(_ts)[rhs]);
			});
		}

		template <std::size_t I, std::size_t... Is, class F>
		void multi_sort(F&& bop) const
		{
			using namespace std;

			_is.resize(get<I>(_ts).size());
			iota(begin(_is), end(_is), 0);

			std::sort(begin(_is), end(_is), [&](size_t lhs, size_t rhs) {
				return detail::use_comparator<I, Is...>::apply(
					_ts, lhs, rhs, forward<F>(bop));
			});
		}

		template <std::size_t I = 0>
		auto size() const
		{
			return std::get<I>(_ts).size(); 
		}

		template <std::size_t I>
		decltype(auto) at(std::size_t pos) 
		{ 
			return std::get<I>(_ts)[_is[pos]]; 
		}
	};

	template <class... Args>
	auto make_sorted_view(Args&&... args)
	{
		return sorted_view<Args...>(std::forward<Args>(args)...);
	}
} // ~namespace utl

#endif

