# `raw_ptr`



## docs [MiraclePtr aka raw_ptr aka BackupRefPtr](https://chromium.googlesource.com/chromium/src/+/ddc017f9569973a731a574be4199d8400616f5a5/base/memory/raw_ptr.md)



## TODO

### zhihu [Google 的 MiraclePtr 是什么？](https://www.zhihu.com/question/485863275)

[Safer Usage Of C++](https://link.zhihu.com/?target=https%3A//docs.google.com/document/d/e/2PACX-1vRZr-HJcYmf2Y76DhewaiJOhRNpjGHCxliAQTBhFxzv1QTae9o8mhBmDl32CRIuaWZLt5kVeH9e9jXv/pub%23h.ji6or6kpi3sa) 中提到的

> Benefits: UAFs account for around [48% of high-severity security bugs](https://link.zhihu.com/?target=https%3A//docs.google.com/document/d/e/2PACX-1vRZr-HJcYmf2Y76DhewaiJOhRNpjGHCxliAQTBhFxzv1QTae9o8mhBmDl32CRIuaWZLt5kVeH9e9jXv/pub%23h.eoikp3r0cwlf) (and climbing). Improved coding patterns are not a robust solution, since they’re still subject to human error. But they may eliminate some fraction of the bugs. When combined with a robust solution such as a deterministic `MiraclePtr`, they may remove some (non-exploitable) crashes.

> NOTE: 
>
> 上面这段话的大致意思是: `MiraclePtr` 是一种 "Improved coding patterns"，虽然 "Improved coding patterns" 并不是一个 "robust solution"，因为它们 "still subject to human error"，但是它们能够消除 "some fraction of the bugs"。

### zdnet [Chrome willing to take performance hit to prevent use-after-free bugs](https://www.zdnet.com/article/chrome-willing-to-take-performance-hit-to-prevent-use-after-free-bugs/)



### google docs [**MiraclePtr One Pager**](https://docs.google.com/document/d/1pnnOAIz_DMWDI4oIOFoMAqLnf_MZ2GsrJNb_dbQ3ZBg/edit#heading=h.p1fqfm5alk4a)