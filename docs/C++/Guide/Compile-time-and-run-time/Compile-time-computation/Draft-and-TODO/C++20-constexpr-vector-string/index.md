# C++20 constexpr vector string

1、在阅读 pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/) 时，其中提及了这个topic

2、modernescpp [constexpr std::vector and std::string in C++20](https://www.modernescpp.com/index.php/constexpr-vector-and-string-in-c-20) 中，对此进行了深入的分析



## pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)



The future looks bright, however. Even when moving slowly, smart people have been working on several proposals to augment the capabilities of `constexpr` to types like `std::string` or `std::vector`. The idea here is to allow for memory allocations at compile time and later flash the object alongside its managed memory into the data section of the binary.



