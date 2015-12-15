#ifndef CPP_17_HL3_KJPP_25
#define CPP_17_HL3_KJPP_25


#include <array>
#include <utility>
#include <type_traits>


namespace cpp17
{

	namespace detail
	{
		// TODO: Write __invoke 
	}

#if 0
	template <class F, class... Args>
	decltype(auto) invoke(F&& f, Args&&... args)
	{
		return detail::__invoke(std::forward<F>(f), std::forward<Args>(args)...);
	}
#endif // TODO: Activate the invoke interface

	namespace detail
	{
		template <class F, class Tuple, std::size_t... I>
		constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
		{
#if 1
			return (std::forward<F>(f))(std::get<I>(std::forward<Tuple>(t))...);
#else
			return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
#endif // TODO: Implement the second branch
		}
	}

	template <class T>
	struct Valency
	{
		static constexpr std::size_t value = std::tuple_size<T>::value;
	};

	template <class T, std::size_t I>
	struct Valency<std::array<T, I>>
	{
		static constexpr std::size_t value = I;
	};

	template <class F, class C>
	constexpr decltype(auto) apply(F&& f, C&& t)
	{
		return detail::apply_impl(std::forward<F>(f), std::forward<C>(t),
			std::make_index_sequence< Valency<std::decay_t<C> >::value >{});
	}

}


#endif











