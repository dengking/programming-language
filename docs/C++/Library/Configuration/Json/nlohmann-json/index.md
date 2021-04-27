# [nlohmann](https://github.com/nlohmann)/**[json](https://github.com/nlohmann/json)**



## Design goals



### Serious testing. 

Our class is heavily [unit-tested](https://github.com/nlohmann/json/tree/develop/test/src) and covers [100%](https://coveralls.io/r/nlohmann/json) of the code, including all exceptional behavior. Furthermore, we checked with [Valgrind](https://valgrind.org/) and the [Clang Sanitizers](https://clang.llvm.org/docs/index.html) that there are no memory leaks. [Google OSS-Fuzz](https://github.com/google/oss-fuzz/tree/master/projects/json) additionally runs fuzz tests against all parsers 24/7, effectively executing billions of tests so far. To maintain high quality, the project is following the [Core Infrastructure Initiative (CII) best practices](https://bestpractices.coreinfrastructure.org/projects/289).

> NOTE: 
>
> 1、它的做法是非常值得借鉴的

