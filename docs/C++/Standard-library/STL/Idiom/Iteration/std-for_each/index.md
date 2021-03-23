# `std::for_each`



## cppreference [std::for_each](https://en.cppreference.com/w/cpp/algorithm/for_each)

## `std::for_each` VS for-loop

### stackoverflow [Should I use std::for_each?](https://stackoverflow.com/questions/6260600/should-i-use-stdfor-each)

[A](https://stackoverflow.com/a/6260709)

There is an advantage to using `std::for_each` instead of an old school `for` loop (or even the newfangled(新流行的) C++0x range-`for` loop): you can look at the first word of the statement and you know exactly what the statement does.

When you see the `for_each`, you know that the operation in the lambda is performed exactly once for each element in the range (assuming no exceptions are thrown). It isn't possible to break out of the loop early before every element has been processed and it isn't possible to skip elements or evaluate the body of the loop for one element multiple times.

With the `for` loop, you have to read the entire body of the loop to know what it does. It may have `continue`, `break`, or `return` statements in it that alter the control flow. It may have statements that modify the iterator or index variable(s). There is no way to know without examining the entire loop.

Herb Sutter discussed the advantages of using algorithms and lambda expressions [in a recent presentation to the Northwest C++ Users Group](http://herbsutter.com/2011/05/20/my-lambdas-talk-nwcpp-is-now-online/).

Note that you can actually use the `std::copy` algorithm here if you'd prefer:

```C++
std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, "\n"));
```

> NOTE: 总的来说:
>
> 1) 相比于`for` loop，`std::for_each`的含义是更加精准、明确，符合[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，能够让code更加得readable
>
> 2) 符合 "Prefer algorithm over hand-write loop" 原则

## tutorialcup [C++ for_each Loop](https://www.tutorialcup.com/cplusplus/for-each-loop.htm)



### Array

```c++
#include 
#include 
using namespace std;
void fun1(int x) 
{
    cout << x << " ";
}
struct Class1 // object type function
{           
    void operator() (int x) 
    {
        cout << x << " ";
    }
} obj1;
int main() 
{
    int arr[] = {11, 21, 4, 13};
    for_each(arr, arr + 4, fun1);
    cout << endl;
    for_each(arr, arr + 4, obj1);
    return 0;
}
```

### Vector

```C++
#include 
#include 
using namespace std;
void fun1(int x) 
{
    cout << x << " ";
}
struct Class1 // object type function
{           
    void operator() (int x) 
    {
        cout << x << " ";
    }
} obj1;
int main() 
{
    vector v1;
    v1.push_back(11);
    v1.push_back(23);
    v1.push_back(4);
    v1.push_back(13);
    for_each(v1.begin(), v1.end(), fun1);        
    cout << endl;
  for_each(v1.begin(), v1.end(), obj1);        
    return 0;
}
```



### Exception in for_each

```C++
#include 
#include 
using namespace std;
void fun1(int x) 
{
    cout << x << " ";
    if (x % 2 == 0)
    {
        throw 100;
    }
}
struct Class1 // object type function
{           
    void operator() (int x) 
    {
        cout << x << " ";
        if (x % 2 == 0)
        {
            throw 100;
        }            
    }
} myobject;
int main() 
{
    vector v1;
    v1.push_back(11);
    v1.push_back(23);
    v1.push_back(4);
    v1.push_back(13);
    try 
    {
        for_each(v1.begin(), v1.end(), fun1);        
    } 
    catch (int i) 
    {
        cout << endl << "Got exception...Value thrown is " << i << endl;
    }
    try 
    {
        for_each (v1.begin(), v1.end(), myobject);
    } 
    catch (int i) 
    {
        cout << endl << "Got exception...Value thrown is " << i << endl;
    }    
    return 0;
}
```





### Advantages of for_each loop

1. Allows us to write an algorithm on top of for_each that works with any iterator.
2. Silly typing bugs can be reduced.
3. for_each is more generic than ‘for loop’ as we can use it to iterate over any type of container.
4. for_each makes more readable code.
5. Performance: Code containing for_each are more efficient.



## Example

fluentcpp [On Design Patterns in C++](https://www.fluentcpp.com/2020/12/18/on-design-patterns-in-cpp/) # Strategy with `std::for_each`

```C++
#include <vector>
#include <algorithm>
#include <iostream>
int main()
{
	auto numbers = std::vector<int> { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::for_each(begin(numbers), end(numbers), [](int &i)
	{	i *= 10;});
	// number now contains { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90 }
	for (auto &&number : numbers)
	{
		std::cout << number << std::endl;
	}

}

```

stackoverflow [How to use lambda in for_each?](https://stackoverflow.com/questions/11502523/how-to-use-lambda-in-for-each)

[A](https://stackoverflow.com/a/11502622)

The third parameter of `for_each` is a function to apply to each *element*, not to each *index*. Otherwise, what would be the point of using that over a traditional loop?

So, instead of an `int` parameter, it takes a `Point` parameter. And now there's no reason to capture anything, because a reference to `PtList` is unnecessary.

```cpp
// Should make operator() const as it doesn't modify anything
for_each(PtList.begin(),PtList.end(),[](Point const& p)
{
    p();
});
```

