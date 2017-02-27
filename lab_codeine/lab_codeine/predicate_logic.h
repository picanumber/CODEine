#ifndef PREDICATE_LOGIC_H_18_12_16
#define PREDICATE_LOGIC_H_18_12_16

#if defined(_MSC_VER) && !defined(__clang__)
#include <utility>
#include <type_traits>
#endif

namespace plogic
{
#if defined(_MSC_VER) && !defined(__clang__)
	namespace detail
	{
		template <bool... Bs> using bool_sequence = 
			std::integer_sequence<bool, Bs...>;

		template <bool... Bs> using bool_and = 
			std::is_same<bool_sequence<Bs... >, bool_sequence<(Bs || true)...>>;

		template <bool... Bs> using bool_or = 
			std::integral_constant<bool, !bool_and< !Bs... >::value>;
	}

	template <bool... Bs> constexpr bool all_v = detail::bool_and<Bs...>::value; 
	template <bool... Bs> constexpr bool any_v = detail::bool_or<Bs...>::value; 
#else
	template <bool... Bs> constexpr bool all_v = (... && Bs);
	template <bool... Bs> constexpr bool any_v = (... || Bs);
#endif
	template <bool... Bs> constexpr bool non_v = !any_v<Bs...>;

	// ------------------------------------ "is there ?"
	template <template <class> class P, class... Ts>
	constexpr bool existential_quantifier = any_v<(P<Ts>::value)...>;

	// ----------------------------------------- "are all ?"
	template <template <class> class P, class... Ts>
	constexpr bool universal_quantifier = all_v<(P<Ts>::value)...>;

	// ----------------------------------------- metafunction currying
	template <template <class...> class C, class...Ts>
	struct curry
	{
		template <class T>
		using type = C<Ts..., T>;
	};

	// Standard library implementations
	template<class...> struct conjunction : std::true_type { };
	template<class B1> struct conjunction<B1> : B1 { };
	template<class B1, class... Bn>
	struct conjunction<B1, Bn...>
		: std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

	template<class...> struct disjunction : std::false_type { };
	template<class B1> struct disjunction<B1> : B1 { };
	template<class B1, class... Bn>
	struct disjunction<B1, Bn...>
		: std::conditional_t<bool(B1::value), B1, disjunction<Bn...>> { };
}


#endif
