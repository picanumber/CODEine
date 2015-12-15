#ifndef ANY_FOLD_HPP_25L3KJ
#define ANY_FOLD_HPP_25L3KJ


namespace fld
{

	template <class F, class T>
	struct XX
	{
		T data;

		template <class D>
		constexpr XX(D&& data)
			: data(std::forward<D>(data))
		{ }

		constexpr T give() const
		{
			return data;
		}
	};

	template <class L, class F, class R>
	struct YY
	{
		L l;
		R r;

		template <class LL, class RR>
		constexpr YY(LL&& l, RR&& r)
			: l(std::forward<LL>(l))
			, r(std::forward<RR>(r))
		{ }

		constexpr auto give() const
		{
			return F{}(l.give(), r.give());
		}
	};

	template <class F, class R, class T>
	constexpr auto operator+(XX<F, T> const& lhs, R const& rhs)
	{
		return YY< XX<F, T>, F, R >(lhs, rhs);
	}

	template <class F, class T1, class T2, class R>
	constexpr auto operator+(YY<T1, F, T2> const& lhs, R const& rhs)
	{
		return YY< YY<T1, F, T2>, F, R >(lhs, rhs);
	}

	namespace detail
	{
		template <class... Ts>
		constexpr auto _foldl(Ts&&... args)
		{
			return (args + ...);
		}

		template <class... Ts>
		constexpr auto _foldr(Ts&&... args)
		{
			return (... + args);
		}
	}

	template <class F, class... Ts>
	constexpr decltype(auto) foldl(Ts&&... args)
	{
		return detail::_foldl(XX<F, Ts>(args)...).give();
	}

	template <class F, class... Ts>
	constexpr decltype(auto) foldr(Ts&&... args)
	{
		return detail::_foldr(XX<F, Ts>(args)...).give();
	}

}


#endif







