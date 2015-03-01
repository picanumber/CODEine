# CODEine
being a code junkie


integer sequence
---
compile time integer tools

Example usage :

```c++
#include <iostream>
#include <CODEine/integer_sequence.h>

using namespace std; 
using namespace cti; 

template<int... Is>
void func(ints<Is...>&& s)
{
	for (auto i : { Is... }) cout << i << " ";
	cout << endl;
}

int main()
{	
	func(sequence<5>());
	func(reverse_sequence<5>());
	func(range<2, 6, 2>());
}
```

prints

```c++
0 1 2 3 4 5
5 4 3 2 1 0
2 4 6
```
