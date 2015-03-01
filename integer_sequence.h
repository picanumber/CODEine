#ifndef I_SEQ_2908SDGF375
#define I_SEQ_2908SDGF375


namespace seq
{
	// a sequence container ---------------------------------------------
	template<int ...>
	struct ints
	{
	};

	// integer sequence -------------------------------------------------
	template<int N, int... Is>
	struct int_seq : int_seq < N - 1, N, Is... >
	{
	};

	template<int... Is>
	struct int_seq < 0, Is... >
	{
		using type = ints < 0, Is... > ;
	};
	
	// convenience alias ----------------------------
	template<int N>
	using IS = typename int_seq<N>::type;
	// ------------------------------------------------------------------

	// reverse integer sequence -----------------------------------------
	template<int C, int N, int... Is>
	struct rev_seq : rev_seq < C - 1, N, N - C, Is... >
	{
	};

	template<int N, int... Is>
	struct rev_seq < 0, N, Is... >
	{
		using type = ints < N, Is... > ;
	};
	
	// convenience alias ----------------------------
	template<int N>
	using RS = typename rev_seq<N, N>::type;
	// ------------------------------------------------------------------

	// integer range ----------------------------------------------------
	template<int N0, int N, int... Is>
	struct int_range : int_range < N0, N - 1, N, Is... >
	{
	};

	template<int N0, int... Is>
	struct int_range < N0, N0, Is... >
	{
		using type = ints < N0, Is... >;
	};

	// convenience alias ----------------------------
	template<int N0, int N>
	using IR = typename int_seq<N0, N>::type;
	// ------------------------------------------------------------------
}


#endif
