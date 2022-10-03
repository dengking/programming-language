# Previous、next



## Iterator operation: previous、next

对应cppreference iterator operation。在阅读akrzemi1 [Value semantics](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/)的时候，其中对比了[std::next](https://en.cppreference.com/w/cpp/iterator/next)、[std::advance](https://en.cppreference.com/w/cpp/iterator/advance)。

### cppreference [std::advance](https://en.cppreference.com/w/cpp/iterator/advance)



### cppreference [std::prev](https://en.cppreference.com/w/cpp/iterator/prev)



#### Notes

Although the expression `--c.end()` often compiles, it is not guaranteed to do so: `c.end()` is an rvalue expression, and there is no iterator requirement that specifies that decrement of an rvalue is guaranteed to work. In particular, when iterators are implemented as pointers or its operator`--` is **lvalue-ref-qualified**, `--c.end()` does not compile, while `std::prev(c.end())` does.

> NOTE:
>
> 一、上面这段话描述了使用 `std::prev` 的原因，简而言之:
>
> 1、"`c.end()` is an rvalue expression"
>
> 2、"when iterators are implemented as pointers or its operator`--` is **lvalue-ref-qualified**, `--c.end()` does not compile, while `std::prev(c.end())` does."

#### Example

```c++
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
    std::vector<int> v{3, 1, 4};

    auto it = v.end();
    auto pv = std::prev(it, 2);
    std::cout << *pv << '\n';

    it = v.begin();
    pv = std::prev(it, -2);
    std::cout << *pv << '\n';
}
```



### cppreference [std::next](https://en.cppreference.com/w/cpp/iterator/next)





### Value semantic and reference semantic

在akrzemi1 [Value semantics](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/) 的Why we use value semantics段，使用value semantic and reference semantic分析iterator operation，下面总结了使用value semantic and reference semantic来分析上述几种operation：

value semantic：[std::prev](https://en.cppreference.com/w/cpp/iterator/prev)、[std::next](https://en.cppreference.com/w/cpp/iterator/next)

reference semantic：[std::advance](https://en.cppreference.com/w/cpp/iterator/advance)



```c++
i = next(i, 1); // value semantic
advance(i, 1);  // reference semantic
```

`advance(i, 1)`是pass-by-reference，在它的内部能够就地修改。



## Sorted container: previous、next

这个topic是我在学习 geeksforgeeks [Given n line segments, find if any two segments intersect](https://www.geeksforgeeks.org/given-a-set-of-line-segments-find-if-any-two-segments-intersect/) 时，其中的写法如下：

```

```



### stackoverflow [C++: Map, previous item of a key](https://stackoverflow.com/questions/20215463/c-map-previous-item-of-a-key)

I have this map: `map<int, int > items`. Given a key, I want that this map returns the item corrisponding to the key if it present, otherwise the map returns the item with key immediately less than the given key. For example, if I have:

```cpp
  items[0]=0;
  items[6]=10;
  items[15]=18;
  items[20]=22;
```

than for key=15, I want that the map returns item with value 18, otherwise for key=9, I want that map returns item with value 10.

I haven't find a function for this case. But I tried in this way:

```cpp
itlow=items.lower_bound(key);
if(!items.count(key))
   itlow--;
return itlow->second;
```

This works as I want, entering in the map a min value `items[0]=0` for default, but I know that `itlow--;` it's not good programming. How can I do? thanks all.



#### [A](https://stackoverflow.com/a/20215827/10173843)



You just need to check if your `itlow` is already `items.begin()`. If it is, there's no such element in the map:

```cpp
itlow=items.lower_bound(key);
if(itlow->first == key)
    return itlow->second;
else if(itlow != items.begin())
    itlow--;
    return itlow->second;
else
    throw some_exception();
```

Instead of throwing exception, you may return iterator, and then you can return `items.end()` if no such element is found.

```cpp
#include <iostream>
#include <map>

using namespace std;


map<int, int>::const_iterator find(const map<int, int> &items, int value)
{
    auto itlow = items.lower_bound(value);

    if(itlow->first == value)
        return itlow;
    else if(itlow != items.cbegin())
        return --itlow;
    else 
        return items.cend();

}

int main()
{
  map<int, int> items;
          items[2]=0;
  items[6]=10;
  items[15]=18;
  items[20]=22;

  auto i = find(items, 0);
  if(i != items.cend())
  {
     cout << i->second << endl;
  }
  i = find(items, 15);
  if(i != items.cend())
  {
    cout << i->second << endl;
  }
  i = find(items, 9);
  if(i != items.cend())
  {
    cout << i->second << endl;
  }
}
```