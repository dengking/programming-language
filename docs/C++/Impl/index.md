# From source code to executable

“From source code to exec”的含义是“从源代码到可执行程序”，这是本章要探讨的问题。按照在 工程[compiler principle](https://dengking.github.io/compiler-principle) 的 "1.1 Language Processors[#](https://dengking.github.io/compiler-principle/Chapter-1-Introduction/1.1-Language-Processors/#11-language-processors)"中所定义的概念，本章所要讨论的是c/c++语言的language processing system。

本章首先采用总分的思路来描述from source code to executable的过程: 首先描述完整的translation流程，建立起鸟瞰的视角，然后描述流程中的各个步骤。

| 章节                  | 简介                                        |
| --------------------- | ------------------------------------------- |
| Phases-of-translation | 描述完整的translation流程，建立起鸟瞰的视角 |
| Preprocess            | 预处理阶段                                  |
| Compile               | 编译阶段                                    |
| Link                  | 链接阶段                                    |



## 将ABI归入本章

本节描述的是我将ABI归入到本章的原因：

一个完整的language processor，比如gcc，需要具备front end和back end

- front end：language grammar

- back end：采用何种ABI

显然，需要将ABI的内容归入到from-source-to-exec中去。