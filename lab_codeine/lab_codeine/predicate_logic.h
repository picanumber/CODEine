#ifndef PREDICATE_LOGIC_H_18_12_16
#define PREDICATE_LOGIC_H_18_12_16


namespace plogic
{
	template <bool... Bs> constexpr bool all_v = (... && Bs);
	template <bool... Bs> constexpr bool any_v = (... || Bs);
	template <bool... Bs> constexpr bool non_v = !any_v<Bs...>;

	// ------------------------------------ "is there ?"
	template <template <class> class P, class... Ts>
	constexpr bool existential_quantifier = any_v<P<Ts>::value...>;

	// ----------------------------------------- "are all ?"
	template <template <class> class P, class... Ts>
	constexpr bool universal_quantifier = all_v<P<Ts>::value...>;

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
