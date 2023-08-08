# `std::vector`



## cppreference [std::vector](https://en.cppreference.com/w/cpp/container/vector)



## Initialization、copy

一、stackoverflow [C++, copy set to vector](https://stackoverflow.com/questions/5034211/c-copy-set-to-vector)

[A](https://stackoverflow.com/a/5034274)

You need to use a `back_inserter`:

```cpp
std::copy(input.begin(), input.end(), std::back_inserter(output));
```

`std::copy` doesn't add elements to the container into which you are inserting: it can't; it only has an iterator into the container. Because of this, if you pass an output iterator directly to `std::copy`, you must make sure it points to a range that is at least large enough to hold the input range.

`std::back_inserter` creates an output iterator that calls `push_back` on a container for each element, so each element is inserted into the container. Alternatively, you could have created a sufficient number of elements in the `std::vector` to hold the range being copied:

```cpp
std::vector<double> output(input.size());
std::copy(input.begin(), input.end(), output.begin());
```

Or, you could use the `std::vector` range constructor:

```cpp
std::vector<double> output(input.begin(), input.end()); 
```

[A](https://stackoverflow.com/a/5034315)

Just use the constructor for the vector that takes iterators:

```cpp
std::set<T> s;

//...

std::vector v( s.begin(), s.end() );
```

[A](https://stackoverflow.com/a/23529333)

here's another alternative using `vector::assign`:

```cpp
theVector.assign(theSet.begin(), theSet.end());
```



二、stackoverflow [Vector of Vectors to create matrix](https://stackoverflow.com/questions/12375591/vector-of-vectors-to-create-matrix)

[A](https://stackoverflow.com/a/12377502)

You have to initialize the vector of vectors to the appropriate size before accessing any elements. You can do it like this:

```cpp
// assumes using std::vector for brevity
vector<vector<int>> matrix(RR, vector<int>(CC));
```

This creates a vector of `RR` size `CC` vectors, filled with `0`.