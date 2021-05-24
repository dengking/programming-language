# `size_t` length

`size_t` length是implementation defined，因此，programmer不能够对其进行预期。

## stackoverflow [Is size_t the word size?](https://stackoverflow.com/questions/14792068/is-size-t-the-word-size)

A

In the C++ standard, [support.types] (18.2) /6: "The type `size_t` is an implementation-defined unsigned integer type that is large enough to contain the size in bytes of any object."

This may or may not be the same as a "word size", whatever that means.