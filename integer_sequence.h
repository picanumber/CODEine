#ifndef I_SEQ_2908SDGF375
#define I_SEQ_2908SDGF375

#include <type_traits>

namespace cti 
{ // compile time integers

	// a sequence container ---------------------------------------------
	template<int ...>
	struct ints { };

	namespace detail
	{
		// integer sequence ---------------------------------------------
		template<int N, int... Is>
		struct int_seq : int_seq < N - 1, N, Is... > { };

		template<int... Is>
		struct int_seq<0, Is...>
		{
			using type = ints < 0, Is... > ;
		};

		// reverse integer sequence -------------------------------------
		template<int C, int N, int... Is>
		struct rev_seq : rev_seq<C - 1, N, N - C, Is...> { };

		template<int N, int... Is>
		struct rev_seq<0, N, Is...>
		{
			using type = ints<N, Is...> ;
		};

		// integer range ------------------------------------------------
		template<int C, int N0, int N, int... Is>
		struct int_range : 
			int_range<C, N0, (N - C > N0 ? N - C : N0), N, Is...> { };

		template<int C, int N0, int... Is>
		struct int_range<C, N0, N0, Is...>
		{
			using type = ints<N0, Is...>;
		};
	}

	// convenience aliases ----------------------------------------------
	template<int N>
	using sequence = typename detail::int_seq<N>::type;
	
	template<int N>
	using reverse_sequence = typename detail::rev_seq<N, N>::type;

	template<int N0, int N, 
		typename std::enable_if<(N0 <= N), int>::type C = 1>
	using range = typename detail::int_range<C, N0, N - ((N-N0)%C)>::type;

} // ~compile time integers

#endif

