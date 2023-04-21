# How to find the second to last element in a vector in C++

## stackoverflow [How to find the second to last element in a vector in C++?](https://stackoverflow.com/questions/18539954/how-to-find-the-second-to-last-element-in-a-vector-in-c)



### [A](https://stackoverflow.com/a/18540244)

Many of the answers and comments have the right idea but really ugly syntax. Here are two nice ways to express that.

```cpp
arr2.end()[-2] // end() is past the last element, -1 for last element, -2 for second-last
arr2.rbegin()[1] // rbegin() is reverse order starting at 0 for last element, 1 for second-last
```

Demo: http://ideone.com/2cZeUq

It works because RandomAccessIterator, which `vector` has, is required to provide `operator[]` such that `it[n]` is equivalent to `*(it + n)`, just like for pointers.

So the code in your question becomes just

```cpp
if (arr2.rbegin()[1]) // test penultimate element
```