#ifndef PERFORMANCE_TESTS_H_20022017
#define PERFORMANCE_TESTS_H_20022017

#include <vector>
#include <memory>
#include <cassert>
#include <unordered_set>
#include <CODEine/benchmark.h>

namespace ptest
{
	template <
		std::size_t steps,
		std::size_t samples,
		class data_t, 
		class bmark_t
	>
	void cs_vector_unique(std::vector<data_t> const& v, bmark_t& bm)
	{
		using timeout_t = 
			bmk::timeout<typename bmark_t::time_t, typename bmark_t::clock_t>;

		std::size_t rec = 0; 

		// 1. create the factors according to the steps (resolution)
		std::vector<std::size_t> factors; 
		std::size_t sz = v.size(), step = v.size() / steps; 
		
		factors.push_back(step); 
		while (factors.back() + step < sz)
		{
			factors.push_back(factors.back() + step); 
		}
		factors.push_back(sz); 

		// 1.
		bm.run("sort & unique", samples, [&](std::size_t num)
		{
			assert(num <= v.size()); // not enough data

			auto to = std::make_unique<timeout_t>(); 			
			to->tic();
			auto ite = v.begin(); 
			std::advance(ite, num); 
			std::vector<data_t> xv(v.begin(), ite); 
			to->toc();

			std::sort(xv.begin(), xv.end()); 
			xv.erase(std::unique(xv.begin(), xv.end()), xv.end()); 

			to->tic();
			rec = num - xv.size(); 
			to->toc();
			
			return to; 
		}, "elements", factors.begin(), factors.end());
		std::cout << "method1 erased " << rec << " duplicates at max factor\n"; 

		// 2. 
		bm.run("set & copy", samples, [&](std::size_t num)
		{
			assert(num <= v.size()); // not enough data

			auto to = std::make_unique<timeout_t>();
			to->tic();
			auto ite = v.begin();
			std::advance(ite, num);
			std::vector<data_t> xv(v.begin(), ite);
			to->toc();

			std::set<data_t> temp(xv.begin(), xv.end()); 
			xv.assign(temp.begin(), temp.end()); 

			to->tic();
			rec = num - xv.size();
			to->toc();

			return to;
		}, "elements", factors.begin(), factors.end());
		std::cout << "method2 erased " << rec << " duplicates at max factor\n";

		// 3. 
		bm.run("unordered set & copy", samples, [&](std::size_t num)
		{
			assert(num <= v.size()); // not enough data

			auto to = std::make_unique<timeout_t>();
			to->tic();
			auto ite = v.begin();
			std::advance(ite, num);
			std::vector<data_t> xv(v.begin(), ite);
			to->toc();

			std::unordered_set<data_t> temp(xv.begin(), xv.end());
			xv.assign(temp.begin(), temp.end());

			to->tic();
			rec = num - xv.size();
			to->toc();

			return to;
		}, "elements", factors.begin(), factors.end());
		std::cout << "method3 erased " << rec << " duplicates at max factor\n";

		// 4. 
		bm.run("unordered set & copy & sort", samples, [&](std::size_t num)
		{
			assert(num <= v.size()); // not enough data

			auto to = std::make_unique<timeout_t>();
			to->tic();
			auto ite = v.begin();
			std::advance(ite, num);
			std::vector<data_t> xv(v.begin(), ite);
			to->toc();

			std::unordered_set<data_t> temp(xv.begin(), xv.end());
			xv.assign(temp.begin(), temp.end());
			std::sort(xv.begin(), xv.end()); 

			to->tic();
			rec = num - xv.size();
			to->toc();

			return to;
		}, "elements", factors.begin(), factors.end());
		std::cout << "method4 erased " << rec << " duplicates at max factor\n";
	}
}

#endif // !PERFORMANCE_TESTS_H_20022017

