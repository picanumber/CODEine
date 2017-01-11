#include "sorted_view.h"

#include <vector>
#include <string>
#include <utility>
#include <iostream>


/*    0     |     1      |       2       |     3      |     4     | 5 | 6  | 7 | 8 | 9  |
|-----------+------------+---------------+------------+-----------+---+----+---+---+----|
| Michael   | Phelps	 | United States | Swimming	  | 2004–2016 | M | 23 | 3 | 2 | 28 |
| Larisa    | Latynina	 | Soviet Union	 | Gymnastics | 1956–1964 | F | 9  | 5 | 4 | 18 |
| Paavo     | Nurmi	     | Finland	     | Athletics  | 1920–1928 | M | 9  | 3 | 0 | 12 |
| Mark      | Spitz	     | United States | Swimming   | 1968–1972 | M | 9  | 1 | 1 | 11 |
| Carl      | Lewis	     | United States | Athletics  | 1984–1996 | M | 9  | 1 | 0 | 10 |
| Usain     | Bolt	     | Jamaica	     | Athletics  | 2008–2016 | M | 9  | 0 | 0 | 9  |
| Ole Einar | Bjorndalen | Norway	     | Biathlon	  | 1998–2014 | M | 8  | 4 | 1 | 13 |
| Bjorn     | Daehlie	 | Norway	     | C/c skiing | 1992–1998 | M | 8  | 4 | 0 | 12 |
| Birgit    | Fischer	 | Germany	     | Canoeing   | 1980–2004 | F | 8  | 4 | 0 | 12 |
| Sawao     | Kato	     | Japan	     | Gymnastics | 1968–1976 | M | 8  | 3 | 1 | 12 |
| Jenny     | Thompson	 | United States | Swimming   | 1992–2004 | F | 8  | 3 | 1 | 12 */

using namespace std;

vector<string> first_name{ "Michael", "Larisa", "Paavo", "Mark", 
	"Carl", "Usain", "Ole Einar", "Bjorn", "Birgit", "Sawao", "Jenny" 
};

vector<string> last_name{ "Phelps", "Latynina", "Nurmi", "Spitz", 
	"Lewis", "Bolt", "Bjorndalen", "Daehlie", "Fischer", "Kato", "Thompson" 
};

vector<string> country{ "United States", "Soviet Union", "Finland", "United States",
	"United States", "Jamaica", "Norway", "Norway", "Germany", "Japan", "United States" 
};

vector<string> sport{ "Swimming", "Gymnastics", "Athletics", "Swimming", 
	"Athletics", "Athletics", "Biathlon", "C/c skiing", "Canoeing", "Gymnastics", "Swimming" 
};

vector<pair<size_t, size_t>> years{ {2004, 2016}, {1956, 1964}, {1920, 1928}, {1968, 1972}, 
	{1984, 1996}, {2008, 2016}, {1998, 2014}, {1992, 1998}, {1980, 2004}, {1968, 1976}, {1992, 2004} 
};

vector<char>   gender       { 'M', 'F', 'M', 'M', 'M', 'M', 'M', 'M', 'F', 'M', 'F' };
vector<size_t> medals_gold  {  23,   9,   9,   9,   9,   9,   8,   8,   8,   8,   8 };
vector<size_t> medals_silver{   3,   5,   3,   1,   1,   0,   4,   4,   4,   3,   3 };
vector<size_t> medals_bronze{   2,   4,   0,   1,   0,   0,   1,   0,   0,   1,   1 };
vector<size_t> medals_total {  28,  18,  12,  11,  10,   9,  13,  12,  12,  12,  12 };


struct S
{
	int i;
	S(int i) : i(i) {}
	int size() { return 1; }
	S(S const&) = delete;
	S(S&&) = default; 
};

int test_sorted_view()
{
	using namespace utl; 

	auto sv = make_sorted_view(first_name, last_name, country, sport,
		years, gender, medals_gold, medals_silver, medals_bronze, medals_total);

	sv.sort<0>(std::less<>{});
	for (size_t i = 0; i < sv.size<0>(); i++) printf("{ %s-%s-%s }\n",
		sv.at<0>(i).c_str(), sv.at<1>(i).c_str(), sv.at<2>(i).c_str());


	sv.multi_sort<9, 6, 7, 8>([](auto&& a, auto&& b) { return a > b; });
	std::cout << "\n\n";
	for (size_t i = 0; i < sv.size<9>(); i++) printf("{ %s-%s-%s }\n",
		sv.at<0>(i).c_str(), sv.at<1>(i).c_str(), sv.at<2>(i).c_str());

	
	return 0; 
}