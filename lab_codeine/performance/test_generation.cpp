#include "datasets.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace 
{
	template <class T>
	void serialize_distribution(std::vector<T> const& distribution_values, const char *distributionName)
	{
		std::ofstream out(std::string("datafiles/") + distributionName + std::string(".txt"));
		out << "{ \"" << distributionName << "\": [";
		for (auto val : distribution_values)
		{
			out << val << ", ";
		}
		out << "] }";
	}

	std::string distribution_header(const char *distrName, std::string const& distrSz)
	{
		std::stringstream ss;
		ss << distrName << " - " << distrSz << " numbers";
		return ss.str();
	}

	// uniform_int_distribution
	void test_uniform_int_distribution(std::vector<int>& v, int low, int high)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::uniform_int_distribution<int>>(low, high));

		serialize_distribution(v, 
			distribution_header("uniform_int_distribution", std::to_string(v.size())).c_str());
	}

	// uniform_real_distribution
	void test_uniform_real_distribution(std::vector<double>& v, int low, int high)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::uniform_real_distribution<double>>(low, high));

		serialize_distribution(v,
			distribution_header("uniform_real_distribution", std::to_string(v.size())).c_str());
	}

	// bernoulli_distribution
	void test_bernoulli_distribution(std::vector<int>& v, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::bernoulli_distribution>(p));

		serialize_distribution(v,
			distribution_header("bernoulli_distribution", std::to_string(v.size())).c_str());
	}

	// binomial_distribution
	void test_binomial_distribution(std::vector<int>& v, int t, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::binomial_distribution<int>>(t, p));

		serialize_distribution(v,
			distribution_header("binomial_distribution", std::to_string(v.size())).c_str());
	}

	// negative_binomial_distribution
	void test_negative_binomial_distribution(std::vector<int>& v, int t, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::negative_binomial_distribution<int>>(t, p));

		serialize_distribution(v,
			distribution_header("negative_binomial_distribution", std::to_string(v.size())).c_str());
	}

	// geometric_distribution
	void test_geometric_distribution(std::vector<int>& v, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::geometric_distribution<int>>(p));

		serialize_distribution(v,
			distribution_header("geometric_distribution", std::to_string(v.size())).c_str());
	}

	// poisson_distribution
	void test_poisson_distribution(std::vector<int>& v, double mean)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::poisson_distribution<int>>(mean));

		serialize_distribution(v,
			distribution_header("poisson_distribution", std::to_string(v.size())).c_str());
	}

	// exponential_distribution: defined for T{ float, double, double }
	void test_exponential_distribution(std::vector<double>& v, double lambda)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::exponential_distribution<double>>(lambda));

		serialize_distribution(v,
			distribution_header("exponential_distribution", std::to_string(v.size())).c_str());
	}

	// gamma_distribution: defined for T{ float, double, long double }
	void test_gamma_distribution(std::vector<double>& v, double alpha, double beta)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::gamma_distribution<double>>(alpha, beta));

		serialize_distribution(v,
			distribution_header("gamma_distribution", std::to_string(v.size())).c_str());
	}

	// weibull_distribution: defined for T{ float, double, long double }
	void test_weibull_distribution(std::vector<double>& v, double shape, double scale)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::weibull_distribution<double>>(shape, scale));

		serialize_distribution(v,
			distribution_header("weibull_distribution", std::to_string(v.size())).c_str());
	}

	// extreme_value_distribution: defined for T{ float, double, long double }
	void test_extreme_value_distribution(std::vector<double>& v, double location, double scale)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::extreme_value_distribution<double>>(location, scale));

		serialize_distribution(v,
			distribution_header("extreme_value_distribution", std::to_string(v.size())).c_str());
	}

	// normal_distribution: defined for T{ float, double, long double }
	void test_normal_distribution(std::vector<double>& v, double mean, double stddev)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::normal_distribution<>>(mean, stddev));

		serialize_distribution(v,
			distribution_header("normal_distribution", std::to_string(v.size())).c_str());
	}

	// lognormal_distribution
	// chi_squared_distribution
	// cauchy_distribution
	// fisher_f_distribution
	// student_t_distribution
	// Sampling distributions
	// discrete_distribution
	// piecewise_constant_distribution
	// piecewise_linear_distribution

}

void dtset::test_dataset_generator()
{
	std::vector<int> vi(100'000); 
	std::vector<double> vd(100'000); 
	
	//test_uniform_int_distribution(vi, -100, 100); 
	//test_bernoulli_distribution(vi, 0.25); 
	//test_binomial_distribution(vi, 10, 0.25); 
	//test_negative_binomial_distribution(vi, 10, 0.25); 
	//test_geometric_distribution(vi, 0.9);
	//test_poisson_distribution(vi, 10);
	//test_exponential_distribution(vd, 1.); 
	//test_gamma_distribution(vd, 0.5, 1.5); 
	//test_weibull_distribution(vd, 10.5, 1.5); 
	//test_extreme_value_distribution(vd, 50, 1.6); 
	test_normal_distribution(vd, 0, 50); 
}
























