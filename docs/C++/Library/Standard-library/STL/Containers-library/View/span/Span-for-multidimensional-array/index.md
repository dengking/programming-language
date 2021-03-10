# Span-for-multidimensional-array



## stackoverflow [CPPCoreGuidelines span for a T** interface?](https://stackoverflow.com/questions/41880770/cppcoreguidelines-spant-for-a-t-interface)

In digital signal processing audio is commonly passed around as a 2D array of channels and samples i.e.

```cpp
void use(float** buffer, int channels, int samples) { ... }
```

A lot of libraries I use expect this format.

In terms of my own code, for safer manipulation, is there a way I could use `gsl::span<T>` to set up views on these buffers?

(I understand 1D but I'm not sure how I would set up for a 2D `float**` array)

Thanks in advance.

### Comment

Use `gsl::multispan` (not well documented) – [Galik](https://stackoverflow.com/users/3807729/galik) Jan 26 '17 at 19:25



## stackoverflow [What is gsl::multi_span to be used for?](https://stackoverflow.com/questions/45201524/what-is-gslmulti-span-to-be-used-for)

The C++ core guidelines mention spans, not "multi-spans". But - I see that Microsoft's GSL implementation has a [`multi_span` class](https://github.com/Microsoft/GSL/blob/master/include/gsl/multi_span)

```c++
template <
    typename ValueType,
    std::ptrdiff_t FirstDimension,
    std::ptrdiff_t... RestDimensions
>
class multi_span { ... };
```

So, obviously this is some sort of a multi-dimensional version of `gsl::span`. But what is that supposed to mean? Why do we need this multi-dimensional span, or rather - when would we use it? I can't seem to find any documentation on this.

### A

In short, it is a span over contiguous piece of memory, which represents multidimensional array.

Here is an example of use:

```c++
int data[6] = {0, 1, 2, 3, 4, 5};
multi_span<int, 2, 3> span{data, 6};
std::cout << span[1][1] << '\n'; //Outputs 4
```

From the linked source, it seems, that it also supports runtime bounds, but I am not sure about proper syntax for those.

## library [nitronoid](https://github.com/nitronoid)/**[multi_span](https://github.com/nitronoid/multi_span)**