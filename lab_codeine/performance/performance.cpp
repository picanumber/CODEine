// performance.cpp : Defines the entry point for the console application.
//

#include "CODEine/benchmark.h"
#include "datasets.h"
#include <cassert>
#include <iterator>
#include <algorithm>
#include <set>

#include "performance_tests.h"


static int case_study_vector_unique()
{
	bmk::benchmark<> bmark1, bmark2;

	std::vector<unsigned long long> ints;
	dtset::dataset_from_file("datafiles/ull_ints.txt", ints, 5'000'000);
	std::cout << "acquired " << ints.size() << " integers\n";
	ptest::cs_vector_unique<10, 10>(ints, bmark1);
	bmark1.serialize("unique a vector of unsigned ints", "results/vector_unique_ull_ints.txt");

	std::vector<std::string> words;
	dtset::dataset_from_file("datafiles/text.txt", words, 5'000'000);
	std::cout << "acquired " << words.size() << " strings\n";
	ptest::cs_vector_unique<10, 10>(words, bmark2);
	bmark2.serialize("unique a vector of strings", "results/vector_unique_strings.txt");

	return 0; 
}

static int case_study_loop()
{
	bmk::benchmark<> bmark1, bmark2;

	std::vector<unsigned long long> ints;
	dtset::dataset_from_file("datafiles/ull_ints.txt", ints, 5'000'000);
	std::cout << "acquired " << ints.size() << " integers\n";
	ptest::cs_loop_vector<10, 1000>(ints, bmark1); 
	bmark1.serialize("loop a vector of unsigned ints", "results/vector_loop_ull_ints.txt");

	std::vector<std::string> words;
	dtset::dataset_from_file("datafiles/text.txt", words, 5'000'000);
	std::cout << "acquired " << words.size() << " strings\n";
	ptest::cs_loop_vector<10, 1000>(words, bmark2);
	bmark2.serialize("loop a vector of strings", "results/vector_loop_strings.txt");

	return 0; 
}

static int case_study_front_insert()
{
	bmk::benchmark<> bmark; 


	std::vector<unsigned long long> ints;
	dtset::dataset_from_file("datafiles/ull_ints.txt", ints, 5000);
	std::cout << "acquired " << ints.size() << " integers\n";

	ptest::cs_seq_front_insert<10, 1>(ints, bmark); 
	bmark.serialize("insert to the front of a sequence", "results/seq_front_insert.txt");
	return 0; 
}

int main()
{
	int dummy; 
	std::cout << "waiting for you to set the t.a.\n"; 
	std::cin >> dummy; 

	case_study_front_insert(); 

    return 0;
}

#if 0
std::cout << "with steady \n";
{
	dtset::random_generator<> rnd(10, 100);
	for (size_t i = 0; i < 5; i++)
	{
		std::cout << rnd() << std::endl;
	}

	std::vector<int> v(14);

	std::generate(v.begin(), v.end(), dtset::random_generator<>(10, 100));

}

std::cout << "\nw/o steady\n";
{
	dtset::random_generator<std::uniform_int_distribution<int>, std::random_device> rnd(10, 100);
	for (size_t i = 0; i < 5; i++)
	{
		std::cout << rnd() << std::endl;
	}
}
#endif // 0

