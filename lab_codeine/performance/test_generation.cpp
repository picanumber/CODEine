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

	// Uniform distributions ----------------------------------------------------------------------
	void test_uniform_int_distribution(std::vector<int>& v, int low, int high)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::uniform_int_distribution<int>>(low, high));

		serialize_distribution(v, 
			distribution_header("uniform_int_distribution", std::to_string(v.size())).c_str());
	}

	void test_uniform_real_distribution(std::vector<double>& v, int low, int high)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::uniform_real_distribution<double>>(low, high));

		serialize_distribution(v,
			distribution_header("uniform_real_distribution", std::to_string(v.size())).c_str());
	}

	// Bernoulli distributions --------------------------------------------------------------------
	void test_bernoulli_distribution(std::vector<int>& v, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::bernoulli_distribution>(p));

		serialize_distribution(v,
			distribution_header("bernoulli_distribution", std::to_string(v.size())).c_str());
	}

	void test_binomial_distribution(std::vector<int>& v, int t, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::binomial_distribution<int>>(t, p));

		serialize_distribution(v,
			distribution_header("binomial_distribution", std::to_string(v.size())).c_str());
	}

	void test_negative_binomial_distribution(std::vector<int>& v, int t, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::negative_binomial_distribution<int>>(t, p));

		serialize_distribution(v,
			distribution_header("negative_binomial_distribution", std::to_string(v.size())).c_str());
	}

	void test_geometric_distribution(std::vector<int>& v, double p)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::geometric_distribution<int>>(p));

		serialize_distribution(v,
			distribution_header("geometric_distribution", std::to_string(v.size())).c_str());
	}

	// Poisson distributions ----------------------------------------------------------------------
	void test_poisson_distribution(std::vector<int>& v, double mean)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::poisson_distribution<int>>(mean));

		serialize_distribution(v,
			distribution_header("poisson_distribution", std::to_string(v.size())).c_str());
	}

	void test_exponential_distribution(std::vector<double>& v, double lambda)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::exponential_distribution<double>>(lambda));

		serialize_distribution(v,
			distribution_header("exponential_distribution", std::to_string(v.size())).c_str());
	}

	void test_gamma_distribution(std::vector<double>& v, double alpha, double beta)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::gamma_distribution<double>>(alpha, beta));

		serialize_distribution(v,
			distribution_header("gamma_distribution", std::to_string(v.size())).c_str());
	}

	void test_weibull_distribution(std::vector<double>& v, double shape, double scale)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::weibull_distribution<double>>(shape, scale));

		serialize_distribution(v,
			distribution_header("weibull_distribution", std::to_string(v.size())).c_str());
	}

	void test_extreme_value_distribution(std::vector<double>& v, double location, double scale)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::extreme_value_distribution<double>>(location, scale));

		serialize_distribution(v,
			distribution_header("extreme_value_distribution", std::to_string(v.size())).c_str());
	}

	// Normal distributions -----------------------------------------------------------------------
	void test_normal_distribution(std::vector<double>& v, double mean, double stddev)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::normal_distribution<>>(mean, stddev));

		serialize_distribution(v,
			distribution_header("normal_distribution", std::to_string(v.size())).c_str());
	}

	void test_lognormal_distribution(std::vector<double>& v, double log_scale, double shape)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::lognormal_distribution<>>(log_scale, shape));

		serialize_distribution(v,
			distribution_header("lognormal_distribution", std::to_string(v.size())).c_str());
	}

	void test_chi_squared_distribution(std::vector<double>& v, double d_f)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::chi_squared_distribution<>>(d_f));

		serialize_distribution(v,
			distribution_header("chi_squared_distribution", std::to_string(v.size())).c_str());
	}

	void test_cauchy_distribution(std::vector<double>& v, double location, double scale)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::cauchy_distribution<>>(location, scale));

		serialize_distribution(v,
			distribution_header("cauchy_distribution", std::to_string(v.size())).c_str());
	}

	void test_fisher_f_distribution(std::vector<double>& v, double m, double n)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::fisher_f_distribution<>>(m, n));

		serialize_distribution(v,
			distribution_header("fisher_f_distribution", std::to_string(v.size())).c_str());
	}

	void test_student_t_distribution(std::vector<double>& v, double d_f)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::student_t_distribution<>>(d_f));

		serialize_distribution(v,
			distribution_header("student_t_distribution", std::to_string(v.size())).c_str());
	}

	// Sampling distributions ---------------------------------------------------------------------
	void test_discrete_distribution(std::vector<int>& v, std::initializer_list<double> weights)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::discrete_distribution<int>>(weights));

		serialize_distribution(v,
			distribution_header("discrete_distribution", std::to_string(v.size())).c_str());
	}

	// piecewise_constant_distribution
	void test_piecewise_constant_distribution(std::vector<double>& v)
	{
		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::piecewise_constant_distribution<>>());

		serialize_distribution(v,
			distribution_header("piecewise_constant_distribution", std::to_string(v.size())).c_str());
	}

	// piecewise_linear_distribution
	void test_piecewise_linear_distribution(std::vector<double>& v)
	{
		std::vector<double> i{ 0, 5, 10, 15 };
		std::vector<double> w{ 0, 1,   1, 0 };

		std::generate(v.begin(), v.end(),
			dtset::random_generator<std::piecewise_linear_distribution<>>(i.begin(), i.end(), w.begin()));

		serialize_distribution(v,
			distribution_header("piecewise_linear_distribution", std::to_string(v.size())).c_str());
	}
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
	//test_normal_distribution(vd, 0, 50);
	//test_lognormal_distribution(vd, 1, 1);
	//test_chi_squared_distribution(vd, 10);
	//test_cauchy_distribution(vd, 0, 0.01);
	//test_fisher_f_distribution(vd, 10, 10); 
	//test_student_t_distribution(vd, 10); 
	//test_discrete_distribution(vi, { 8, 7, 6, 5, 4, 3, 2, 1, 7.5, 8 }); 
	//test_piecewise_constant_distribution(vd);
	test_piecewise_linear_distribution(vd);
}
























