# C++ and Beyond 2012: Andrei Alexandrescu - Systematic Error Handling in C++

## Abstract

Writing code that is resilient upon errors (API failures, exceptions, invalid memory access, and more) has always been a pain point in all languages. This being still largely an unsolved (and actually rather loosely-defined) problem, C++11 makes no claim of having solved it. However, C++11 is a more expressive language, and as always more expressive features can be put to good use toward devising better error-safe idioms and libraries.

This talk is a thorough visit through error resilience and how to achieve it in C++11. After a working definition, we go through a number of approaches and techniques, starting from the simplest and going all the way to file systems, storage with different performance and error profiles (think HDD vs. RAID vs. Flash vs. NAS), and more. As always, scaling up from in-process to inter-process to cross-machine to cross-datacenter entails different notions of correctness and resilience and different ways of achieving such.

To quote a classic, "one more thing"! An old acquaintance—[ScopeGuard](http://www.drdobbs.com/article/print?articleId=184403758&siteSectionName=cpp)—will be present, with the note that ScopeGuard11 is much better (and much faster) than its former self.

## Slide

> NOTE: channel9 [C++ and Beyond 2012: Andrei Alexandrescu - Systematic Error Handling in C++](https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C) 中给出的slide下载链接无法下载，在paper-N4152中，附带了它的slide。

## TODO

channel9 [C++ and Beyond 2012: Andrei Alexandrescu - Systematic Error Handling in C++](https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C)

https://isocpp.org/blog/2012/12/systematic-error-handling-in-c-andrei-alexandrescu

https://www.developerfusion.com/media/148072/c-and-beyond-2012-andrei-alexandrescu-systematic-error-handling-in-c/

https://cppandbeyond.com/2012/12/