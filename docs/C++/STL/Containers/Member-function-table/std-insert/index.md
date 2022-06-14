# insert

1、`std::copy` + `std::back_inserter`、`std::inserter`、`std::front_inserter`

2、insert函数的返回值是new iterator，这样是能够避免iterator invalidation的

3、需要注意的是，insert是insert before而不是insert after

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

```C++
std::vector<int> v{begin(newElements), end(newElements)};
```

For appending several new elements to an existing vector:

```C++
v.insert(end(v), begin(newElements), end(newElements));
```

Note that these methods also exist for the other STL containers, in particular `std::set` and `std::map`.

Finally, to replace the entire contents of a vector with `newElements`:

```C++
v.assign(begin(newElements), end(newElements));
```

After the execution of `assign`, all the previous elements have been replaced by the new ones, regardless of the respective numbers of new and old elements. But for a reason that I didn’t quite understand, the `assign` method does not exist for associative containers such as `std::set` and `std::map`.



### Is `std::copy` useful at all then?

> NOTE:
>
> 标题的意思是： "那么 `std::copy` 有用吗？"

In the above case, `std::copy` was not appropriate because it blindly extended the size of the container. But sometimes, we don’t extend the size of the container, or we can’t know in advance how many elements are to be added.

For instance, if the container already has values, and we want to **overwrite them starting at the beginning**, we would use `std::copy`:



```c++

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main()
{
    std::vector<int> v = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
    std::vector<int> newElements = {1, 2, 3};

    std::copy(begin(newElements), end(newElements), begin(v));
    // v now contains {1, 2, 3, 5, 5, 5, 5, 5, 5, 5};

    for (int i : v)
        std::cout << i << ' ';
}
```

Another example is writing into a C array:

```c++

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main()
{
    int a[10] = {};
    std::vector<int> newElements = {1, 2, 3};

    std::copy(begin(newElements), end(newElements), std::begin(a));
    // a now contains {1, 2, 3, 0, 0, 0, 0, 0, 0, 0};

    for (int i : a)
        std::cout << i << ' ';
}
```

And we will see an example of a case where we can’t know in advance how many elements are to be added, when we address **stream iterators**, in a dedicated post.

### Is `std::back_inserter` useful at all then?

Yes again!

It is typically useful for adding into a container the result of any algorithm that does more than `std::copy`. For example `std::copy_if`:

```C++

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

int main()
{
    std::vector<int> v;
    std::vector<int> newElements = {1, 3, 2, 4, 3, 2, 2};

    std::copy_if(begin(newElements), end(newElements), std::back_inserter(v), [](int i)
                 { return i % 2 == 0; });

    for (int i : v)
        std::cout << i << ' ';
}
```

## cppreference [std::vector<T,Allocator>::insert](https://en.cppreference.com/w/cpp/container/vector/insert)

> NOTE:
>
> 一、它的返回值是new iterator，这样说能够避免iterator invalidation的
>
> 二、需要注意的是: insert是insert before而不是insert after

### Example

一、fluentcpp [How the STL inserter iterator really works](https://www.fluentcpp.com/2017/10/06/stl-inserter-iterators-work/)

```c++
_Self& operator=(const typename _Container::value_type& __val) {
  _M_iter = container->insert(_M_iter, __val);
  ++_M_iter;
  return *this;
}
```

