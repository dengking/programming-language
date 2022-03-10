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



## 复用chromium llvm toolchain

clang 的 版本和 c++标准库的版本不一致的话，是否有影响？