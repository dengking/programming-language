# swap

swap是非常重要的operation，它可以看做是language-support-function，它是很多其他operation的基石，和`std::move`有的一拼，本章对swap进行总结，主要关注:

1、如何custom swap并达到strong exception safety，即non-throw

2、如何使用swap并"facilitate generic programming"

> 上述两点其实是在 More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap) 中强调的

## 素材

在下面文章中，对`swap`有着非常好的描述

1、open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html)

> analogous(类似于) to the existing verb "swap," which is equivalent to a move, two move-assignments, and a destroy

总结的比较好