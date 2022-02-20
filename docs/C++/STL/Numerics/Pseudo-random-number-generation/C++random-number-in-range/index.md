# C/C++ 中生成特定范围内的随机数

Google "C++ random number in range"

## stackoverflow [Generate random numbers uniformly over an entire range](https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range)

```cpp
for(int i=0; i<6; i++)
{
    DWORD random = rand()%(max-min+1) + min;
}
```

### [A](https://stackoverflow.com/a/20136256/10173843)

#### Why `rand` is a bad idea

Most of the answers you got here make use of the `rand` function and the modulus operator. That method [may not generate numbers uniformly](http://channel9.msdn.com/Events/GoingNative/2013/rand-Considered-Harmful) (it depends on the range and the value of `RAND_MAX`), and is therefore discouraged.

#### C++11 and generation over a range

With C++11 multiple other options have risen. One of which fits your requirements, for generating a random number in a range, pretty nicely: [`std::uniform_int_distribution`](http://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution). Here's an example:

```cpp
#include <random>
#include <iostream>
int main()
{
    const int range_from  = 0;
    const int range_to    = 10;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);

    std::cout << distr(generator) << '\n';
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror

```

And [here](http://coliru.stacked-crooked.com/a/c5b94870fdcd13f2)'s the running example.

Template function may help some:

```cpp
template<typename T>
T random(T range_from, T range_to) {
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<T>    distr(range_from, range_to);
    return distr(generator);
}
```

> NOTE: 
>
> 一、更好的做法如下:
>
> ```C++
> #include <vector>
> #include <algorithm>
> #include <random>
> #include <iostream>
> 
> template<typename T>
> T random(T range_from, T range_to) {
>     static std::random_device                  rand_dev;
>     static std::mt19937                        generator(rand_dev());
>     static std::uniform_int_distribution<T>    distr(range_from, range_to);
>     return distr(generator);
> }
> 
> int main()
> {
> 	std::cout << random(1, 2) << std::endl;
> }
> 
> // g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror
> 
> ```
>
> 

#### Other random generators

The [`header](http://en.cppreference.com/w/cpp/numeric/random) offers innumerable other random number generators with different kind of distributions including Bernoulli, Poisson and normal.

#### How can I shuffle a container?

The standard provides [`std::shuffle`](http://en.cppreference.com/w/cpp/algorithm/random_shuffle), which can be used as follows:

```cpp
#include <vector>
#include <algorithm>
#include <random>
int main()
{
	std::vector<int> vec = { 4, 8, 15, 16, 23, 42 };

	std::random_device random_dev;
	std::mt19937       generator(random_dev());

	std::shuffle(vec.begin(), vec.end(), generator);
}

// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror

```

The algorithm will reorder the elements randomly, with a linear complexity.

#### Boost.Random

Another alternative, in case you don't have access to a C++11+ compiler, is to use [Boost.Random](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_random.html). Its interface is very similar to the C++11 one.

## csdn [C/C++ 中生成特定范围内的随机数](https://blog.csdn.net/Justme0/article/details/41547761)

> NOTE: 
>
> 这篇文章概述了C、C++的做法

### 2 C++ 中的 random 库

```C++
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

/*
** return a random integer in the interval [a, b]
*/
int uniform_int(int a, int b) {
	static std::default_random_engine e{ std::random_device{}() }; // avoid "Most vexing parse"
	static std::uniform_int_distribution<int> u;

	return u(e, decltype(u)::param_type(a, b));
}

/*
** return a random real in the interval [a, b] (also [a, b))
*/
double uniform_real(double a, double b) {
	static std::default_random_engine e{ std::random_device{}() };
	static std::uniform_real_distribution<double> u;

	return u(e, decltype(u)::param_type(a, b));
}

int main()
{
	std::cout << uniform_int(1, 2) << std::endl;
}

// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror

```



## `rand()%range + min`

相比于上面的写法，这种写法非常简单易记。

### cnblogs [C++指定范围生成随机数](https://www.cnblogs.com/vathena/articles/4356646.html)

> NOTE: 
>
> module用于指定随机数的范围

```c++
#include "stdafx.h"
#include <iostream>
#include <ctime>
 
#define Randmod(x) rand()%x
using namespace std;
 
int _tmain(int argc, _TCHAR* argv[])
{
    //根据系统时间设置随机数种子
    srand( (unsigned)time(NULL) );
 
    //随机生成10个[0,10]的数
    for(int i = 0; i < 10; i++ )
    {
         cout << Randmod(11) << endl;
    }
    return 0;
}
```



```c++
srand((unsigned)time(null));
(low,up) #define Random (rand()%(up-low+1)) + low - 1
[low,up) #define Random (rand()%(up-low)) + low
(low,up] #define Random (rand()%(up-low))+ low + 1
[low,up] #define Random (rand()%(up-low+1)) + low
```



### java2blog [How to generate random number between 1 and 10 in C++](https://java2blog.com/random-number-between-1-and-10-cpp/)



```C++
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
int main() 
{   
  srand(time(0));  // Initialize random number generator.
 
  cout<<"Random numbers generated between 1 and 10:"<<endl;
  for(int i=0;i<10;i++)
      cout << (rand() % 10) + 1<<" ";  
  return 0; 
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -Werror
```

