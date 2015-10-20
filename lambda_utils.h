#ifndef LAMBDA_UTILS_20_10_2015
#define LAMBDA_UTILS_20_10_2015

namespace lut
{

	template <class... F>
	struct overload_set : F...
	{
		overload_set(F... f) : F(f)... {}
	};

	template <class... F>
	auto overload(F... f)
	{
		return overload_set<F...>(f...);
	}

	namespace detail
	{
		struct NC
		{
			NC() = default;
			NC(NC const&) = delete;
			NC& operator=(NC const&) = delete;
			NC(NC&&) = default;
		};
	} 

} // ~ namespace lut

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define UNIQUE_IDEXPR(x) CONCATENATE(x, __COUNTER__)

#define nocopy UNIQUE_IDEXPR(x) = lut::detail::NC()

#undef CONCATENATE_DETAIL
#undef CONCATENATE
#undef UNIQUE_IDEXPR

#endif

