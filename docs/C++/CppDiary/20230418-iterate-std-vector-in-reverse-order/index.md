# Iterate C++ vector in reverse order



## stackoverflow [Iterating C++ vector from the end to the beginning](https://stackoverflow.com/questions/3610933/iterating-c-vector-from-the-end-to-the-beginning)



### [A](https://stackoverflow.com/a/3610963)

One way is:

```cpp
for (vector<my_class>::reverse_iterator i = my_vector.rbegin(); 
        i != my_vector.rend(); ++i ) { 
} 
```

`rbegin()`/`rend()` were especially designed for that purpose. (And yes, incrementing a `reverse_interator` moves it backward.)

Now, in theory, your method (using `begin()`/`end()` & `--i`) would work, `std::vector`'s iterator being bidirectional, but remember, `end()` isn't the last element — it's one beyond the last element, so you'd have to decrement first, and you are done when you reach `begin()` — but you still have to do your processing.

```cpp
vector<my_class>::iterator i = my_vector.end();
while (i != my_vector.begin())
{
     --i;
    /*do stuff */

} 
```

UPDATE: I was apparently too aggressive in re-writing the `for()` loop into a `while()` loop. (The important part is that the `--i` is at the beginning.)



## thispointer [C++: Iterate over a Vector in Reverse Order – (backward direction)](https://thispointer.com/c-iterate-over-a-vector-in-reverse-order/)



```c++
#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

int main()
{
    // Create a vector of integers
    vector<int> vec_of_num{1, 3, 4, 7, 8, 9};

    // Iterate over a vector in backward direction using
    // reverse iterators, for_each() and Lambda function
    std::for_each(  vec_of_num.rbegin(), 
                    vec_of_num.rend(),
                    [](const auto & elem){
                        std::cout<<elem<<", "; 
                    });

    return 0;
}
```