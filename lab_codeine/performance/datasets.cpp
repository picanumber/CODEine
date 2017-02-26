#include <iostream> 
#include <fstream>
#include "datasets.h"
#include <iterator>
#include <algorithm>

std::size_t dtset::dataset_from_file(
	const char * filename, std::vector<std::string>& words, std::size_t limit)
{
	std::size_t ret = words.size(); 
	std::ifstream file(filename, std::ios::in);	

#if 0
	if (file.is_open())
	{
		std::string word;
		while (file >> word)
		{
			words.push_back(word);
		}
	}
#else
	std::istream_iterator<std::string> start(file), end;	
	words.insert(words.end(), start, end); 

	if (limit)
	{
		while (words.size() > limit)
		{
			words.pop_back(); 
		}
	}

#endif // 0

	return (words.size() - ret);
}

std::size_t dtset::dataset_from_file(
	const char * filename, std::vector<int>& nums, std::size_t limit)
{
	std::size_t ret = nums.size();
	std::ifstream file(filename, std::ios::in);

	std::istream_iterator<int> start(file), end;
	nums.insert(nums.end(), start, end);

	if (limit)
	{
		while (nums.size() > limit)
		{
			nums.pop_back();
		}
	}

	return (nums.size() - ret);
}

std::size_t dtset::dataset_from_file(
	const char * filename, std::vector<unsigned long long>& nums, std::size_t limit)
{
	std::size_t ret = nums.size();
	std::ifstream file(filename, std::ios::in);

	std::istream_iterator<unsigned long long> start(file), end;
	nums.insert(nums.end(), start, end);

	if (limit)
	{
		while (nums.size() > limit)
		{
			nums.pop_back();
		}
	}

	return (nums.size() - ret);
}

