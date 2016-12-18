#include <iostream>
#include <type_traits>
#include "predicate_logic.h"


template <class... Ts>
constexpr bool are_tc =
plogic::universal_quantifier<std::is_trivially_copyable, Ts...>;

struct S {};

template <class T1, class T2>
using is_smaller = std::integral_constant<bool, sizeof(T1) < sizeof(T2)>;

struct            S0 {};
struct alignas(2) S1 {};
struct alignas(4) S2 {};
struct alignas(8) S3 {};

int predicate_logic()
{
	static_assert(are_tc<S, std::integral_constant<int, 0>>);
	static_assert(plogic::universal_quantifier<plogic::curry<is_smaller, S0>::type, S1, S2, S3>);

	return 0; 
}