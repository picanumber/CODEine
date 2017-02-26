#ifndef DATASETS_HPP_162016
#define DATASETS_HPP_162016

#include <string>
#include <random>
#include <vector>

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
	}

	template <
		class distribution_type = std::uniform_int_distribution<int>,
		class seed_type = detail::steady_engine<int, 1>, /* use std::random_device for non steady seed */
		class engine_type = std::mt19937
	>
		struct random_generator
	{
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

