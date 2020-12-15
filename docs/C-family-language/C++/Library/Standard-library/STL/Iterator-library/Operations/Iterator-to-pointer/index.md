# Iterator to pointer

## 1) stackoverflow [Convert iterator to pointer?](https://stackoverflow.com/questions/743055/convert-iterator-to-pointer)

> NOTE: `&(*it)`

[A](https://stackoverflow.com/a/2160319)

here it is, obtaining a reference to the coresponding pointer of an iterator use :

example:

```C++
#include <string>
int main()
{
	std::string my_str = "hello world";

	std::string::iterator it(my_str.begin());

	char *pointer_inside_buffer = &(*it); //<--
}

```

[notice operator `*` returns a **reference** so doing `&` on a reference will give you the address].

## 2) stackoverflow [converting vector iterator to pointer](https://stackoverflow.com/questions/37345988/converting-vector-iterator-to-pointer)

I have a vector std::vector. I would like to iterate the vector for finding a match, if found would like to return the pointer to the element as below:

```cpp
const int* findint(std::vector<int> &v, int a)
{
	std::vector<int>::const_iterator i1, i2;
	i1 = v.begin();
	i2 = v.end();
	for (; i1 != i2; ++i1)
	{
		if (a == *i1)
		{
			return (i1);
		}
	}
	return (0);
}

```

[A](https://stackoverflow.com/a/37346088)

This will solve your problem:

```cpp
const int* findint(const std::vector<int> &v, int a)
{
	auto i1 = v.cbegin();
	auto i2 = v.cend();
	for (; i1 != i2; ++i1)
	{
		if (a == *i1)
		{
			return &*i1;
		}
	}
	return nullptr;
}

```

**Edit**: Note that I changed iterators to `cbegin` and `cend` also the `vector` is now passed as `const`.

However, the right way to do it IMO (with respect to [nathanoliver](https://stackoverflow.com/users/4342498/nathanoliver) note):

```cpp
auto it = std::find(v.cbegin(),v.cend(),value);
decltype(&*it) ptr;
if(it==v.cend()){
    ptr = nullptr;
}
else{
    ptr = &*it;
}
```

You have to be careful when using this. Pointers and Iterators may be invalid after any `push_back` or `insert` or `erase` on the vector, for a comprehensive list see [Iterator invalidation rules](https://stackoverflow.com/questions/6438086/iterator-invalidation-rules). If you want to keep a clue to reach some item later. and if you can guarantee that only adding to the back of the vector will happen, you may keep the index of the item using:

```cpp
const int* findint(const std::vector<int> &v, int a)
{
	auto it = std::find(v.cbegin(), v.cend(), value);
	decltype(&*it) ptr;
	if (it == v.cend())
	{
		ptr = nullptr;
	}
	else
	{
		ptr = &*it;
	}
	return ptr;
}

```

[A](https://stackoverflow.com/a/37392928)

Use [`v.data()`](http://en.cppreference.com/w/cpp/container/vector/data):

```cpp
const int * findint(const std::vector <int> &v, int a)
{
  const int * const b = v.data();
  const int * const e = b + v.size();
  const int * const r = std::find(b, e, a);
  return (r == e) ? nullptr : r;
}
```

## TODO: 3) artins [Converting an STL vector iterator to a raw pointer](https://ben.artins.org/software/converting-an-stl-vector-iterator-to-a-raw-pointer/)

