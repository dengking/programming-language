# 关于本章

“From source code to exec”的含义是“从源代码到可执行程序”，这是本章要探讨的问题。按照在工程[compiler principle](https://dengking.github.io/compiler-principle)的1.1 Language Processors[#](https://dengking.github.io/compiler-principle/Chapter-1-Introduction/1.1-Language-Processors/#11-language-processors)中所定义的概念，本章所要讨论的是c/c++语言的language processing system。



## cppreference [Phases of translation](https://en.cppreference.com/w/cpp/language/translation_phases)



## creference [Phases of translation](https://en.cppreference.com/w/c/language/translation_phases)



## 将ABI归入本章

本节描述的是我将ABI归入到本章的原因：

一个完整的language processor，比如gcc，需要具备front end和back end

- front end：language grammar

- back end：采用何种ABI

显然，需要将ABI的内容归入到from-source-to-exec中去。