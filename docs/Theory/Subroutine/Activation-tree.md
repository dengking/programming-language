# Activation Tree

每次调用还是都需要new一个栈帧，相当于new了一个node。

参见工程[compiler-principle](https://dengking.github.io/compiler-principle/)的文章[7.2 Stack Allocation of Space](https://dengking.github.io/compiler-principle/Chapter-7-Run-Time-Environments/7.2-Stack-Allocation-of-Space/)。



nesting in time，在时间维度是一条线，即一条path，相当于能够将他们给串联起来，类似于一个chain。貌似能够nesting的，都能够线性化。

函数调用过程看做是production的推导过程，一个函数调用就相当于一个non-terminal，需要进行expand。函数是具备hierarchy结构的：如果将函数中的每个语句看做是一个leaf node，将函数调用看做是一个inner node，每次调用一个函数就相当于expand这个node。则整个函数就形成了一棵树。

函数的执行过程非常类似于`build_nav_tree`中构造整个`nav_tree`的过程。`nav_tree`是nesting in space。

nesting结构是可以线性化的。

本质上是相同的，函数比也可以看做是类似于production的，都是在描述包含关系。一个函数体中调用了哪些函数，则就相当于production在描述它的body。

使用栈来生成树：将parsing的过程和函数执行的过程都看做是按照production生成一棵树。

所有具备nesting结构的都可以使用production来进行描述。这就是normal hierarchy的强大之处所在。这就是pushdown automata和call stack的共同点所在。



## function、hierarchy、cfg
programming language是context free language。具备nesting结构。

函数的definition也具备nesting结构。

都可以使用cfg来进行描述。

可以将函数的definition也看做是cfg：普通语句就相当于terminal，函数调用语句就相当于non-terminal。

则main函数就相当于start symbol。

则整个函数的执行过程就类似于一个自顶向下的parsing。

