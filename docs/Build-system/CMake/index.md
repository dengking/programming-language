# CMake



## [cmake](https://cmake.org/) 官网





## zhihu [CMake 如何入门？](https://www.zhihu.com/question/58949190)

### [曹草艸的回答](https://www.zhihu.com/question/58949190/answer/999701073) 

**如何学习 CMake**

首先反对认为 CMake 不需要入门的观点：

> cmake还需要入门？这种工具性质的东西，你只要用到哪里学到哪里就行了，如果需要入门那就说明这个工具做的不怎么样。

这句话后半句是符合事实的，CMake 这个工具不仅是做的不怎么样，而且做的....一言难尽....以至于这个问题下就有回答说不要跳 CMake 这个坑的；更多的吐槽可以参考 [如何评价CMake](https://www.zhihu.com/question/276415476) 这个问题。

所以自 CMake v3.0 开始（严格的来说是 v3.2[[2\]](#ref_2)），社区开始了浩浩荡荡的 Modern CMake 的运动，试图通过引入一些新的特性并搭配推荐做法来提升用户的生活质量。

嗯，另一个广泛喊着 modernization 的社区刚好是 C++ 社区...

又恰好这俩的现代化用法的相同之处在于需要摒弃老的、传统的观念&使用方法，使用新的、现代化的惯用法。

打一个可能不太准的比喻：说 CMake 用到哪学到哪就行了和说 C++ 用到哪学到哪就行了一样；如果你本身对这块领域已经很有经验，那么这样做是没问题的；但是对于一个新人来说，无异于让他自己去踩雷。

另一个上来就直接扔给对方一个 CMake documentation 让他学的做法也是非常糟糕的；这就好比一个新手说要学习 C++，你直接朝他扔了 cpp reference...

**学习 (Modern) CMake 的合适路径**

**0x00 起手式**

这里假设题主以及其他想入门 CMake 的人像我一样鶸，下面是我个人总结的比较适合的学习路径。

首先默念三遍并记住口诀：

1. Declare a target
2. Declare target's traits
3. It's all about targets

然后 clone [https://github.com/ttroy50/cmake-examples](https://link.zhihu.com/?target=https%3A//github.com/ttroy50/cmake-examples) 这个项目到本地，把里面的

- 01-basic（跳过E-installing，因为和依赖有关，后面会说）
- 02-sub-projects

两个目录认真的学习一遍，最好自己能够动手跟着做一遍。



## zhihu [如何评价 CMake？](https://www.zhihu.com/question/276415476)

### [Xi Yang的回答](https://www.zhihu.com/question/276415476/answer/388142387) 

墙裂赞同 [@pilot](https://www.zhihu.com/people/8f525f56d469c60669186bcb14344649) 的说法。CMake这个玩意，原本是一个GNU [autotools](https://www.zhihu.com/search?q=autotools&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A388142387})类似的替代品，用来解决跨平台的环境差异，然而它自己也没有避免一些一直存在在autotools里的问题。经过一段时间的重度使用，我发现它有几个问题是根本性的：

1：它是基于[过程式编程语言](https://www.zhihu.com/search?q=过程式编程语言&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A388142387})生成项目，而不是描述项目。这产生了一系列的恶果。

- CMakeLists基本不可能像各种XML的[项目文件](https://www.zhihu.com/search?q=项目文件&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A388142387})那样，被IDE作为[配置文件](https://www.zhihu.com/search?q=配置文件&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A388142387})。于是你永远需要手工编写CMakeLists的内容，而不能使用图形界面去配置一个个的目标。这对于开发人员的要求其实是很高的。
- 从CMakeLists里面，你很难看到一个目标**最终**的编译选项、包含路径、[预定义宏](https://www.zhihu.com/search?q=预定义宏&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A388142387})都有那些。如果你想要看到这些，除非你对CMakeLists“求解”，把项目生成出来，在生成的结果里才能看到。而对于[大型项目](https://www.zhihu.com/search?q=大型项目&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A388142387})，[generate](https://www.zhihu.com/search?q=generate&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A388142387})的时间可能很长。于是你就难以**交互地**编辑一个项目，经常需要耗时的修改CMakeLists、生成、编译、查错的循环。



2：它虽然是过程式语言，但函数TMD没有返回值！

函数里返回一个值的方式，是把期望接受返回值的变量名传进去，然后在函数里设置外层作用域的那个变量的值。

```cmake
function(my_func ret_var_name)
    set(${ret_var_name} "return value" PARENT_SCOPE)
endfunction()
```

于是，你不能像正常的语言那样写：

```cmake
foo(bar(baz(input)))
```

而必须写：

```cmake
foo(foo_ret)
bar(foo_ret)
baz(bar_ret)
```





### [邱昊宇的回答](https://www.zhihu.com/question/276415476/answer/537782595) 

C++ 社区在推广 Modern C++ 概念后，又开始帮忙推广 Modern CMake，我觉得也是成功的。

比起早年间看一眼就不想碰的「基于过程式的语言」，现在基于 **target** 描述的模型给人感觉是走上了正道。



### [HIPPIE LIU的回答](https://www.zhihu.com/question/276415476/answer/411167976) 

Modern CMake在target based的开发理念下，其实已经有了相当强的模块化能力。`include` keyword已经淡出了历史舞台，成为legacy。以下两篇blog我觉得针对这个理念写的不错：

[It’s Time To Do CMake Right](https://link.zhihu.com/?target=https%3A//pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)

[https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1](https://link.zhihu.com/?target=https%3A//gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1)



自动解决依赖问题早就不是什么问题了： Package developers应该生成package files给upstream users使用。如果你创建了一个library foo, 理论上别人应当只需要find_package(foo)便足以。

[cmake-packages(7) - CMake 3.12.20180606-g074bb Documentation](https://link.zhihu.com/?target=https%3A//cmake.org/cmake/help/git-master/manual/cmake-packages.7.html%23creating-packages)

Windows上微软出了一个叫[vcpkg](https://www.zhihu.com/search?q=vcpkg&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A411167976})的管理工具，据说挺好用的,可以了解下。



"它虽然是过程式语言，但函数TMD没有返回值!"好问题。。。我午休去问问同事

“作为一个string based language, 很少有利用CMake来处理functional programming的cases. 加上CMake诞生之初要兼容[fortran](https://www.zhihu.com/search?q=fortran&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A411167976}), 所以这一功能也不了了之。 虽然community有几个人提过这个[feature request](https://www.zhihu.com/search?q=feature request&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"answer"%2C"sourceId"%3A411167976}),但是没有一个令人信服的使用场景 ”by Brad King。就这样。

## TODO

### cmake target suffix

stackoverflow [Change Extension of Library?](https://stackoverflow.com/questions/26196926/change-extension-of-library)

[[CMake] Python Extension](https://cmake.org/pipermail/cmake/2006-January/007844.html)