# gcc.gnu [10 `gcov`—a Test Coverage Program](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)

## 10.1 Introduction to `gcov`

`gcov` is a test coverage program. Use it in concert with GCC to analyze your programs to help create more efficient, faster running code and to discover untested parts of your program. You can use `gcov` as a profiling tool to help discover where your optimization efforts will best affect your code. You can also use `gcov` along with the other profiling tool, `gprof`, to assess which parts of your code use the greatest amount of computing time.

> NOTE: 
>
> 主要两个application:
>
> 一、profiler
>
> 开源和  `gprof` 一起使用
>
> 二、coverage

### Performance profile

> NOTE: 
>
> 1、能够提供一些 "basic performance statistic"

### Coverage 

Software developers also use coverage testing in concert with testsuites, to make sure software is actually good enough for a release.

### 使用注意事项

1、Compile your code without optimization

2、macros 



### Output: sourcefile.gcov 

`gcov` creates a logfile called sourcefile.gcov which indicates how many times each line of a source file sourcefile.c has executed.



## [10.2 Invoking `gcov`](https://gcc.gnu.org/onlinedocs/gcc/Invoking-Gcov.html#Invoking-Gcov)



## [10.3 Using `gcov` with GCC Optimization](https://gcc.gnu.org/onlinedocs/gcc/Gcov-and-Optimization.html#Gcov-and-Optimization)



## [10.4 Brief Description of `gcov` Data Files](https://gcc.gnu.org/onlinedocs/gcc/Gcov-Data-Files.html#Gcov-Data-Files)

`gcov` uses two files for profiling. The names of these files are derived from the original *object* file by substituting the file suffix with either `.gcno`, or `.gcda`. 

The files contain **coverage** and **profile** data stored in a platform-independent format. The `.gcno` files are placed in the same directory as the object file. By default, the `.gcda` files are also stored in the same directory as the object file, but the GCC `-fprofile-dir` option may be used to store the .gcda files in a separate directory.

### `.gcno`  `-ftest-coverage`



### `.gcda` `-fprofile-arcs`