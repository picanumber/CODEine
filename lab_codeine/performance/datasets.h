#ifndef DATASETS_HPP_162016
#define DATASETS_HPP_162016

#include <string>
#include <random>
#include <vector>
#include <type_traits>

#include "CODEine/lab_codeine/lab_codeine/predicate_logic.h"

namespace dtset
{
	void test_dataset_generator(); 

	std::size_t dataset_from_file(const char *filename, std::vector<std::string>& words, std::size_t lim = 0); 
	std::size_t dataset_from_file(const char *filename, std::vector<int>& words, std::size_t lim = 0);
	std::size_t dataset_from_file(const char *filename, std::vector<unsigned long long>& words, std::size_t lim = 0);

	namespace detail
	{
		template <class SeedT, SeedT SeedV>
		struct steady_engine
		{
			constexpr SeedT operator()() const
			{
				return SeedV;
			}
		};

		template <class...> 
		struct typelist 
		{};

		using int_typelist = typelist<
			short, int, long, long long, unsigned short, 
			unsigned int, unsigned long, unsigned long long>;
		
		using real_typelist = typelist<float, double, long double>;

		template <class T, class TL> 
		struct in_typelist_impl; 

		template <class T, class... Ts>
		struct in_typelist_impl<T, typelist<Ts...>>
		{
			static constexpr bool value =
				plogic::existential_quantifier<plogic::curry<std::is_same, T>::type, Ts...>;
		};

		template <class T, class TL>
		constexpr bool in_typelist_v = in_typelist_impl<T, TL>::value; 

		template <class>
		struct distribution_checker : std::bool_constant<true>
		{}; 

		template <class T>
		struct distribution_checker<std::uniform_int_distribution<T>> 
			: std::bool_constant<in_typelist_v<T, int_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::uniform_real_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::binomial_distribution<T>>
			: std::bool_constant<in_typelist_v<T, int_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::negative_binomial_distribution<T>>
			: std::bool_constant<in_typelist_v<T, int_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::geometric_distribution<T>>
			: std::bool_constant<in_typelist_v<T, int_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::poisson_distribution<T>>
			: std::bool_constant<in_typelist_v<T, int_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::exponential_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::gamma_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::weibull_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::extreme_value_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::normal_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::lognormal_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::chi_squared_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::cauchy_distribution<T>> 
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::fisher_f_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::student_t_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::discrete_distribution<T>>
			: std::bool_constant<in_typelist_v<T, int_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::piecewise_constant_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};

		template <class T>
		struct distribution_checker<std::piecewise_linear_distribution<T>>
			: std::bool_constant<in_typelist_v<T, real_typelist>>
		{};
	}

	template <
		class distribution_type = std::uniform_int_distribution<int>,
		class seed_type = detail::steady_engine<int, 1>, /* use std::random_device for non steady seed */
		class engine_type = std::mt19937
	>
		struct random_generator
	{
		static_assert(
			detail::distribution_checker<distribution_type>::value, "invalid distribution instantiation");
		// ------------------------------------------------ types
		using distribution_t = distribution_type;
		using seed_t = seed_type;
		using engine_t = engine_type;
		using result_t = typename distribution_t::result_type;
		// ------------------------------------------------ properties
		engine_t _rEngine;
		distribution_t _dis;
		// ------------------------------------------------ construction / destruction
		template <class... Args>
		random_generator(Args&&... distribution_args)
			: _rEngine{ seed_t{}() }
			, _dis{ std::forward<Args>(distribution_args)... }
		{
		}
		// ------------------------------------------------ functionality 
		result_t operator()()
		{
			return _dis(_rEngine);
		}
	};
}

#endif

