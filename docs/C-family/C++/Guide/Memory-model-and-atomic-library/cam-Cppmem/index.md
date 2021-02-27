# Cppmem

在阅读 modernescpp [Ongoing Optimization: Acquire-Release Semantic with CppMem](https://www.modernescpp.com/index.php/ongoing-optimization-acquire-release-semantic-with-cppmem) 时，发现的这个工具。

## cam [CppMem: an interactive C/C++ memory model explorer](http://svr-pes20-cppmem.cl.cam.ac.uk/cppmem/help.html)

CppMem is a tool to let one explore the behaviour of small concurrent test programs in the relaxed memory model of the recent C11 and C++11 revisions of C and C++ [[C++11 public draft N3376 (see 1.10, 29, 30)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3376.pdf)], [[WG14](http://www.open-std.org/JTC1/SC22/wg14/)] [[WG21](http://www.open-std.org/jtc1/sc22/wg21/)], [Boehm and Adve, PLDI 2008](http://www.hpl.hp.com/techreports/2008/HPL-2008-56.html)], as formalised by [Batty et al. [POPL 2011\]](http://www.cl.cam.ac.uk/~pes20/cpp/), which explains the model.