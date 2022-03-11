# libc++

## code

[chromium](https://github.com/chromium/chromium)/[buildtools](https://github.com/chromium/chromium/tree/main/buildtools)/[third_party](https://github.com/chromium/chromium/tree/main/buildtools/third_party)/[**libc++**](https://github.com/chromium/chromium/tree/main/buildtools/third_party/libc%2B%2B)/

[chromium](https://github.com/chromium/chromium)/[buildtools](https://github.com/chromium/chromium/tree/main/buildtools)/[third_party](https://github.com/chromium/chromium/tree/main/buildtools/third_party)/[**libc++abi**](chromium/buildtools/third_party/libc++abi/)/

## build



```shell
gn ls out/Default
```

上述命令会罗列出所有的target，其中就包含了 `//buildtools/third_party/libc++:libc++` ，下面看看这个target:

```
gn desc out/Default //buildtools/third_party/libc++:libc++
```

输出如下：

```
Target //buildtools/third_party/libc++:libc++
type: shared_library
toolchain: //build/toolchain/mac:clang_arm64
outputs
  //out/Default/libc++.dylib
  //out/Default/libc++.dylib.TOC
```



### 单独编译libc++

```
gn clean out/Default
```



```
ninja -C out/Default  buildtools/third_party/libc++:libc++ -v > build_libc++output
```



## TODO

思考：什么地方将它挂载到了build graph上？

