# chromium clang plugin

一、关于chromium中的clang plugin，在doc [Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md) # [Using plugins](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md#using-plugins) 章节中进行了详细的介绍，其中提及了如下clang plugin

1、[chromium style plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors)

2、FindBadConstructs plugin

FindBadConstructs plugin的source code位于 [chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[clang](https://github.com/chromium/chromium/tree/main/tools/clang)/[**plugins**](https://github.com/chromium/chromium/tree/main/tools/clang/plugins)/ 。



二、source code

[chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[clang](https://github.com/chromium/chromium/tree/main/tools/clang)/[**plugins**](https://github.com/chromium/chromium/tree/main/tools/clang/plugins)/



三、[Don't write a clang plugin](https://chromium.googlesource.com/chromium/src/+/main/docs/writing_clang_plugins.md)

这篇文章的含义是: 不到万不得已不要写plugin。