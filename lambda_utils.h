#ifndef LAMBDA_UTILS_20_10_2015
#define LAMBDA_UTILS_20_10_2015

#include <type_traits>

namespace lut
{

	template<class... Fs>
	struct overload_set
	{
	};

	template<class F0, class...Fs>
	struct overload_set<F0, Fs...>
		: F0, overload_set<Fs...>
	{
		overload_set(F0 f0, Fs... fs) :
		F0(std::move(f0)),

		overload_set<Fs...>(std::move(fs)...)
		{
		}

		using F0::operator();
		using overload_set<Fs...>::operator();
	};

	template<class F>
	struct overload_set<F> : F
	{
		overload_set(F f) :F(std::move(f)){};
		using F::operator();
	};

	template<class...Fs>
	overload_set<typename std::decay<Fs>::type...> overload(Fs&&...fs)
	{
		return{ std::forward<Fs>(fs)... };
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

#define nocopy UNIQUE_IDEXPR(x___x) = lut::detail::NC()

#undef CONCATENATE_DETAIL
#undef CONCATENATE
#undef UNIQUE_IDEXPR

#endif

