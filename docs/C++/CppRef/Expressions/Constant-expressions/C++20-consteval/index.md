# C++20 `consteval`

awfulcode [Immediate functions in C++20](https://awfulcode.io/2019/01/26/immediate-functions-in-c20/)

stackoverflow [What is consteval?](https://stackoverflow.com/questions/53347298/what-is-consteval)

[A](https://stackoverflow.com/a/53347377)

It declares *immediate functions*, that is, functions that must be evaluated at compile time to produce a constant. (It used to be spelled `constexpr!` in a previous revision of the paper.) In contrast, `constexpr` functions may be evaluated at compile time or run time, and need not produce a constant in all cases.

The adopted paper is [P1073R3](https://wg21.link/P1073R3), which is not yet publicly available, but [a previous revision](https://wg21.link/P1073R2) is available and the introductory (motivation and high-level description) portion is about the same (except that the "Source Locations" section is deleted in R3).

[cppreference-`consteval` specifier (since C++20)](https://en.cppreference.com/w/cpp/language/consteval)
