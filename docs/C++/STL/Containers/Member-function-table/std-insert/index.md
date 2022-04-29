# insert

1、`std::copy` + `std::back_inserter`、`std::inserter`、`std::front_inserter`



## fluentcpp [Inserting several elements into an STL container efficiently](https://www.fluentcpp.com/2017/03/28/inserting-several-elements-into-an-stl-container/)

### Inserting several elements into an STL container

In the posts concerning the STL, I use output iterators such as `std::back_inserter` quite intensely. While such iterators are very handy, it is important to realize that **in some cases you don’t want to use them**.

And these cases come down to inserting several consecutive elements into an STL container.

> NOTE:
>
> 翻译如下:
>
> "在有关 STL 的帖子中，我非常频繁地使用输出迭代器，例如 std::back_inserter。虽然这样的迭代器非常方便，但重要的是要意识到在某些情况下您不想使用它们。 这些情况归结为将几个连续的元素插入到 STL 容器中。"

The – suboptimal – way to inserting several elements by using `std::back_inserter` is to couple it with `std::copy`:

```c++

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main()
{
    std::vector<int> v;
    std::vector<int> newElements = {1, 3, 4, 2, -7, 8};

    std::copy(begin(newElements), end(newElements), std::back_inserter(v));

    for (int i : v)
        std::cout << i << ' ';
}
```

Here `std::copy` successively passes each of the elements in `newElements` to the output iterator, that adds them to `v` by calling its `push_back` method. And this does the job: after the execution of `std::copy`, all the elements from `newElements` have been effectively copied into `v`.

Knowing in advance how many elements are going to be added can be exploited by the vector. This lets it minimize the number of reallocations during the operation: it would reallocate once and for all before starting the operation, instead of reallocating several times along the multiple unitary calls to `push_back`.

> NOTE: 
>
> 这是STL的performance optimization。

![img](https://www.fluentcpp.com/wp-content/uploads/2017/03/20170328_inserting_ranges_image.png)

So, how can we benefit from this information while inserting into a vector? Just by using the **range insertion methods**.

At initialization of the vector, use the range constructor:

```
std::vector<int> v{begin(newElements), end(newElements)};
```

For appending several new elements to an existing vector:

```
v.insert(end(v), begin(newElements), end(newElements));
```

Note that these methods also exist for the other STL containers, in particular `std::set` and `std::map`.

Finally, to replace the entire contents of a vector with newElements:

```
v.assign(begin(newElements), end(newElements));
```

After the execution of `assign`, all the previous elements have been replaced by the new ones, regardless of the respective numbers of new and old elements. But for a reason that I didn’t quite understand, the `assign` method does not exist for associative containers such as `std::set` and `std::map`.