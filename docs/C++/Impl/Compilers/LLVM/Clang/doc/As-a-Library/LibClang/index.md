# `libclang`



https://llvm.org/devmtg/2010-11/Gregor-libclang.pdf

https://sudonull.com/post/907-An-example-of-parsing-C-code-using-libclang-in-Python

## llvm [**libclang: C Interface to Clang**](https://clang.llvm.org/doxygen/group__CINDEX.html)

> NOTE: 
>
> 一、在 libclang 中，无法使用 [LibTooling](https://clang.llvm.org/docs/LibTooling.html)、[LibASTMatcher](https://clang.llvm.org/docs/LibASTMatchers.html) 中的功能，正如下面所介绍的 "The C Interface to Clang provides a relatively small API"。
>
> 二、原文将libclang的C interface分为了几个"Module"



## libclang comment

stackoverflow [How to extract comments and match to declaration with RecursiveASTVisitor in libclang c++?](https://stackoverflow.com/questions/25275212/how-to-extract-comments-and-match-to-declaration-with-recursiveastvisitor-in-lib)

[A](https://stackoverflow.com/a/41736499)

**"-fparse-all-comments"**



itbaoku [如何使用libclang c ++中的RecursiveASTVisitor提取注释并与声明匹配?](https://www.itbaoku.cn/post/1816672/How-to-extract-comments-and-match-to-declaration-with-RecursiveASTVisitor-in-libclang-c)

stackoverflow [Retrieving comments using python libclang](https://stackoverflow.com/questions/19079070/retrieving-comments-using-python-libclang)



## libclang  pre-processor

stackoverflow [Retrieve information about pre-processor directives](https://stackoverflow.com/questions/13881506/retrieve-information-about-pre-processor-directives)





## Compile string

stackoverflow [Using libclang to parse in C++ in Python](https://stackoverflow.com/questions/36808565/using-libclang-to-parse-in-c-in-python) # [A](https://stackoverflow.com/a/36821958/10173843)

```python
import clang.cindex

s = '''
int fac(int n) {
    return (n>1) ? n*fac(n-1) : 1;
}
'''

idx = clang.cindex.Index.create()
tu = idx.parse('tmp.cpp', args=['-std=c++11'],  
                unsaved_files=[('tmp.cpp', s)],  options=0)
for t in tu.get_tokens(extent=tu.cursor.extent):
    print t.kind
```

