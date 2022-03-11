# llvm toolchain



## chromium llvm toolchain

### 如何指定linker？

在 stackoverflow [CMake: use a custom linker](https://stackoverflow.com/questions/1867745/cmake-use-a-custom-linker) # [A](https://stackoverflow.com/a/64174822) :

> cmake -DCMAKE_CXX_FLAGS="-fuse-ld=lld"

在 [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/main/build)/[config](https://github.com/chromium/chromium/tree/main/build/config)/[compiler](https://github.com/chromium/chromium/tree/main/build/config/compiler)/[**BUILD.gn**](https://github.com/chromium/chromium/blob/main/build/config/compiler/BUILD.gn) 中有实际的例子。



### C++标准库

它自己编译c++标准库，它不使用系统的c++库，关于此，在 [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/main/build)/[config](https://github.com/chromium/chromium/tree/main/build/config)/[c++](https://github.com/chromium/chromium/tree/main/build/config/c%2B%2B)/[**BUILD.gn**](https://github.com/chromium/chromium/blob/main/build/config/c%2B%2B/BUILD.gn) 中，有如下说明: 

```c++
    # Make sure we don't link against the system libstdc++ or libc++.
    if (is_clang) {
      ldflags += [ "-nostdlib++" ]
    } else {
      # Gcc has a built-in abs() definition with default visibility.
      # If it was not disabled, it would conflict with libc++'s abs()
      # with hidden visibility.
      cflags += [ "-fno-builtin-abs" ]

      ldflags += [ "-nodefaultlibs" ]

      # Unfortunately, there's no way to disable linking against just libc++
      # (gcc doesn't have -notstdlib++:
      # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83931); -nodefaultlibs
      # removes all of the default libraries, so add back the ones that we need.
      libs += [
        "c",
        "gcc_s",
        "m",
        "rt",
      ]
    }
```

动态链接还是静态链接？



## 如何指定使用什么compiler？

在官方文档 [Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md) 中关于 "Using a custom clang binary" 有如下说明

> Set `clang_base_path` in your `args.gn` to the llvm build directory containing `bin/clang` (i.e. the directory you ran cmake). This must be an absolute path. You also need to disable chromium's clang plugin.
>
> Here's an example that also disables debug info and enables the component build (both not strictly necessary, but they will speed up your build):
>
> ```python
> clang_base_path = getenv("HOME") + "/src/llvm-build"
> clang_use_chrome_plugins = false
> is_debug = false
> symbol_level = 1
> is_component_build = true
> ```

这说明是通过 `clang_base_path` 来指定使用的compiler的，通过 `clang_use_chrome_plugins` 来指定是否使用chromium plugin。

那 `clang_base_path` 在什么地方指定的呢？通过在VS code这进行全文检索，找到了:

```sh
build/config/clang/clang.gni
```

上述文件会被其他文件依赖以确定compiler的位置，这通过在全文检索 `clang_base_path` 可以得知。

`clang_use_chrome_plugins` 也是在 `build/config/clang/clang.gni` 中定义的。

### plugin

在官方文档 [Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md) 中关于"Using plugins"的说明

> The [chromium style plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) is used by default when clang is used.
>
> If you're working on the plugin, you can build it locally like so:
>
> 1. Run `./tools/clang/scripts/build.py --without-android --without-fuchsia` to build the plugin.
> 2. Run `ninja -C third_party/llvm-build/Release+Asserts/` to build incrementally.
> 3. Build with clang like described above, but, if you use goma, disable it.
>
> To test the FindBadConstructs plugin, run:
>
> ```
> (cd tools/clang/plugins/tests && \
>  ./test.py ../../../../third_party/llvm-build/Release+Asserts/bin/clang)
> ```

读完了上面这段话，有如下疑问: 

一、上述"FindBadConstructs plugin"是否就是 [chromium style plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) ？

上面的话，还给出了一个提示是: chromium使用 [`/tools/clang/scripts/build.py`](https://github.com/chromium/chromium/blob/main/tools/clang/scripts/build.py) 来编译clang。因此关于这个问题，我看了一下

1、[`/tools/clang/scripts/build.py`](https://github.com/chromium/chromium/blob/main/tools/clang/scripts/build.py) 

在其中发现了如下有价值的内容:

```python
  default_tools = ['plugins', 'blink_gc_plugin', 'translation_unit']
  chrome_tools = list(set(default_tools + args.extra_tools))
```

上述 'plugins'、'blink_gc_plugin'、'translation_unit'都是对应的 [chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[**clang**](https://github.com/chromium/chromium/tree/main/tools/clang)/ 下的目录

2、 [chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[**clang**](https://github.com/chromium/chromium/tree/main/tools/clang)/[**plugins**](https://github.com/chromium/chromium/tree/main/tools/clang/plugins)/

发现，其中其实并没有style plugin的，倒是有 [FindBadConstructs](https://github.com/chromium/chromium/blob/main/tools/clang/plugins/FindBadConstructsConsumer.h) ，再结合 [`/tools/clang/scripts/build.py`](https://github.com/chromium/chromium/blob/main/tools/clang/scripts/build.py) 中关于编译相关的内容可知， [chromium](https://github.com/chromium/chromium)/[tools](https://github.com/chromium/chromium/tree/main/tools)/[**clang**](https://github.com/chromium/chromium/tree/main/tools/clang)/[**plugins**](https://github.com/chromium/chromium/tree/main/tools/clang/plugins)/应该就包含了"FindBadConstructs plugin"，应该也就包含了  [chromium style plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) 。



#### `find_bad_constructs`

在 `build/config/clang/BUILD.gn` 中定义了它的build:

```javascript
config("find_bad_constructs") {
  if (clang_use_chrome_plugins) {
    cflags = []

    # The plugin is built directly into clang, so there's no need to load it
    # dynamically.
    cflags += [
      "-Xclang",
      "-add-plugin",
      "-Xclang",
      "find-bad-constructs",
      "-Xclang",
      "-plugin-arg-find-bad-constructs",
      "-Xclang",
      "raw-ptr-template-as-trivial-member",
      "-Xclang",
      "-plugin-arg-find-bad-constructs",
      "-Xclang",
      "use-classify-type",
    ]

    if (is_linux || is_chromeos || is_android || is_fuchsia) {
      cflags += [
        "-Xclang",
        "-plugin-arg-find-bad-constructs",
        "-Xclang",
        "check-ipc",
      ]
    }
  }
}
```

其中的注释:

```
# The plugin is built directly into clang, so there's no need to load it
# dynamically.
```

非常重要。



### mac上进行编译

一、下面上过程的记录:

1、

```
gn gen out/Default

```

2、

```
vi out/Default/args.gn
```



```
is_debug = false
is_component_build = true
symbol_level = 0
target_cpu = "arm64" # 可选
```

关于此，参见 [Checking out and building Chromium for Mac # Faster builds](https://chromium.googlesource.com/chromium/src/+/main/docs/mac_build_instructions.md#faster-builds)

3、

```
autoninja -C out/Default chrome -v > output
```

`-v` 的目的是能够看到完整的编译命令，关于此参见stackoverflow [Output calls to clang when running a build.ninja file](https://stackoverflow.com/questions/44681899/output-calls-to-clang-when-running-a-build-ninja-file) # [A](https://stackoverflow.com/a/44696810) 。



二、`llvm-ar`

docs llvm-ar - LLVM archiver[¶](https://llvm.org/docs/CommandGuide/llvm-ar.html#llvm-ar-llvm-archiver)



## 复用chromium llvm toolchain

clang 的 版本和 c++标准库的版本不一致的话，是否有影响？

