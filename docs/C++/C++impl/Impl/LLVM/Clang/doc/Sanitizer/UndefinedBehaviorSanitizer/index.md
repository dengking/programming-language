# [UndefinedBehaviorSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)

> NOTE: 
>
> 1、符合"error handling-report error on time not run on error-及时报告错误-而不是在错误的基础上运行"
>
> 2、C++标准对于undefined behavior仅仅停留在语言层面，实际application中，如果发生了undefined behavior，程序并不会抛出exception等通知用户，而是将在错误的基础上进行运行，将会导致更加难以排查的错误

## [Introduction](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html#id1)

UndefinedBehaviorSanitizer (UBSan) is a fast undefined behavior detector. UBSan modifies the program at compile-time to catch various kinds of undefined behavior during program execution, for example:

- Using misaligned or null pointer
- Signed integer overflow
- Conversion to, from, or between floating-point types which would overflow the destination

