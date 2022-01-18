# Type decay

本文讨论C family language中的type decay。

## Decay in implicit type conversion

本节介绍的implicit type conversion是自动发生的。

### Array to pointer decay

参见: 

1) cppreference [Array to pointer conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion#Array_to_pointer_conversion)

2) cppreference [Array-to-pointer decay](https://en.cppreference.com/w/cpp/language/array#Array-to-pointer_decay)

3) `C-and-C++\Pointer-and-array\Array`章节

### Function to pointer

参见:

1) cppreference [Function to pointer](https://en.cppreference.com/w/cpp/language/implicit_conversion#Function_to_pointer)

## `std::decay`

在进行generic programming的时候，`std::decay`能够实现上述 "decay in implicit type conversion"，参见:

1) cppreference [std::decay](https://en.cppreference.com/w/cpp/types/decay)

2) `C++\Library\Standard-library\Utility-library\Language-support\Type-support\Type-traits\Type-transformations\decay`章节