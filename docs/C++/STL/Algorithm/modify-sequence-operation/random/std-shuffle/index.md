# std::shuffle



## cppreference [std::random_shuffle, std::shuffle](https://en.cppreference.com/w/cpp/algorithm/random_shuffle)







Note that the implementation is not dictated by the standard, so even if you use exactly the same `RandomFunc` or `URBG` (Uniform Random Number Generator) you may get different results with different standard library implementations.

The reason for removing `std::random_shuffle` in C++17 is that the iterator-only version usually depends on [std::rand](https://en.cppreference.com/w/cpp/numeric/random/rand), which is now also discussed for deprecation. ([std::rand](https://en.cppreference.com/w/cpp/numeric/random/rand) should be replaced with the classes of the [<`random`>](https://en.cppreference.com/w/cpp/header/random) header, as [std::rand](https://en.cppreference.com/w/cpp/numeric/random/rand) is *considered harmful*.) In addition, the iterator-only `std::random_shuffle` version usually depends on a global state. The `std::shuffle`'s shuffle algorithm is the preferred replacement, as it uses a `URBG` as its 3rd parameter.

> NOTE:
>
> 一、[std::random_shuffle](https://en.cppreference.com/w/cpp/algorithm/random_shuffle) 已经被淘汰，使用 [std::shuffle](https://en.cppreference.com/w/cpp/algorithm/random_shuffle) 



### Example: shuffle vector

```c++
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
 
int main()
{
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
 
    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(v.begin(), v.end(), g);
 
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}
```

### Example: shuffle array

stackoverflow [Is it possible to random_shuffle an array of int elements?](https://stackoverflow.com/questions/14720134/is-it-possible-to-random-shuffle-an-array-of-int-elements)



```c++
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>

int main() {
    int array[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(std::begin(array), std::end(array), g);

    std::copy(std::begin(array), std::end(array), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}
```



### Example: shuffle `std::array`

[cplusplus-std::shuffle](https://cplusplus.com/reference/algorithm/shuffle/)

```c++

// shuffle algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

int main() {
    std::array<int, 5> foo{1, 2, 3, 4, 5};

    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle(foo.begin(), foo.end(), std::default_random_engine(seed));

    std::copy(foo.begin(), foo.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

```



### Possible implementation



```c++
// 采用Fisher–Yates-shuffle算法
void shuffleArray(vector<int>& nums) {
	srand(time(0));
	int len = nums.size();
	for (int i = 0; i < len; ++i) {
		int j = rand() % (len - i);
		swap(nums[i], nums[j]);
	}
}
```
