

# CMake使用LLVM toolchain

在 [cmake-examples](https://github.com/ttroy50/cmake-examples)/[01-basic](https://github.com/ttroy50/cmake-examples/tree/master/01-basic)/[**D-shared-library**](https://github.com/ttroy50/cmake-examples/tree/master/01-basic/D-shared-library)/ 的基础上进行了修改，目的是使用chromium llvm toolchain，除此之外，它还参考了：

1、[cmake-examples](https://github.com/ttroy50/cmake-examples)/[01-basic](https://github.com/ttroy50/cmake-examples/tree/master/01-basic)/[**I-compiling-with-clang**](https://github.com/ttroy50/cmake-examples/tree/master/01-basic/I-compiling-with-clang)/

其中展示了如何指定toolchain

| variable             |                                      | 
| -------------------- | ------------------------------------ | 
| `CMAKE_C_COMPILER`   | The program used to compile c code   | 
| `CMAKE_CXX_COMPILER` | The program used to compile c++ code | 
| `CMAKE_LINKER`       | The program used to link your binary | 



## 运行结果

目前增加了如下内容：

```cmake
set(CMAKE_C_COMPILER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang")
set(CMAKE_CXX_COMPILER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++")
set(CMAKE_LINKER "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/lld")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
```



```SHELL
(base) dengkai01@dengkaideMacBook-Pro build % make VERBOSE=1
/opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -S/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library -B/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build --check-build-system CMakeFiles/Makefile.cmake 0
/opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E cmake_progress_start /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build/CMakeFiles /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build//CMakeFiles/progress.marks
/Applications/Xcode-beta.app/Contents/Developer/usr/bin/make  -f CMakeFiles/Makefile2 all
/Applications/Xcode-beta.app/Contents/Developer/usr/bin/make  -f CMakeFiles/hello_library.dir/build.make CMakeFiles/hello_library.dir/depend
cd /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build && /opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E cmake_depends "Unix Makefiles" /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build/CMakeFiles/hello_library.dir/DependInfo.cmake --color=
/Applications/Xcode-beta.app/Contents/Developer/usr/bin/make  -f CMakeFiles/hello_library.dir/build.make CMakeFiles/hello_library.dir/build
[ 25%] Building CXX object CMakeFiles/hello_library.dir/src/Hello.cpp.o
/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++ -Dhello_library_EXPORTS -I/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/include -fuse-ld=lld -v -arch arm64 -isysroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -fPIC -MD -MT CMakeFiles/hello_library.dir/src/Hello.cpp.o -MF CMakeFiles/hello_library.dir/src/Hello.cpp.o.d -o CMakeFiles/hello_library.dir/src/Hello.cpp.o -c /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/src/Hello.cpp
clang version 15.0.0 (https://github.com/llvm/llvm-project/ 5bec1ea7a74895895e7831fd951dd8130d4f3d01)
Target: arm64-apple-darwin21.4.0
Thread model: posix
InstalledDir: /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin
clang: warning: argument unused during compilation: '-fuse-ld=lld' [-Wunused-command-line-argument]
 (in-process)
 "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang" -cc1 -triple arm64-apple-macosx12.0.0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name Hello.cpp -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=2 -target-sdk-version=12.3 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -target-cpu apple-m1 -target-feature +v8.5a -target-feature +fp-armv8 -target-feature +neon -target-feature +crc -target-feature +crypto -target-feature +dotprod -target-feature +fp16fml -target-feature +ras -target-feature +lse -target-feature +rdm -target-feature +rcpc -target-feature +zcm -target-feature +zcz -target-feature +fullfp16 -target-feature +sha2 -target-feature +aes -target-abi darwinpcs -fallow-half-arguments-and-returns -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=lldb -target-linker-version 609.8 -v -fcoverage-compilation-dir=/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build -resource-dir /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0 -dependency-file CMakeFiles/hello_library.dir/src/Hello.cpp.o.d -MT CMakeFiles/hello_library.dir/src/Hello.cpp.o -sys-header-deps -isysroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -D hello_library_EXPORTS -I /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/include -stdlib=libc++ -internal-isystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/../include/c++/v1 -internal-isystem /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/local/include -internal-isystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/include -internal-externc-isystem /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/include -fdeprecated-macro -fdebug-compilation-dir=/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build -ferror-limit 19 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/hello_library.dir/src/Hello.cpp.o -x c++ /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/src/Hello.cpp
clang -cc1 version 15.0.0 based upon LLVM 15.0.0git default target x86_64-apple-darwin21.4.0
ignoring nonexistent directory "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/local/include"
ignoring nonexistent directory "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/Library/Frameworks"
#include "..." search starts here:
#include <...> search starts here:
 /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/include
 /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/../include/c++/v1
 /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/include
 /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/include
 /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/System/Library/Frameworks (framework directory)
End of search list.
[ 50%] Linking CXX shared library libhello_library.dylib
/opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E cmake_link_script CMakeFiles/hello_library.dir/link.txt --verbose=1
/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++  -fuse-ld=lld -v -arch arm64 -isysroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -dynamiclib -Wl,-headerpad_max_install_names -o libhello_library.dylib -install_name @rpath/libhello_library.dylib CMakeFiles/hello_library.dir/src/Hello.cpp.o 
clang version 15.0.0 (https://github.com/llvm/llvm-project/ 5bec1ea7a74895895e7831fd951dd8130d4f3d01)
Target: arm64-apple-darwin21.4.0
Thread model: posix
InstalledDir: /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin
 "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/ld64.lld" -demangle -dynamic -dylib -arch arm64 -dylib_install_name @rpath/libhello_library.dylib -platform_version macos 12.0.0 12.3 -syslibroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -o libhello_library.dylib -headerpad_max_install_names CMakeFiles/hello_library.dir/src/Hello.cpp.o -lc++ -lSystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/lib/darwin/libclang_rt.osx.a
[ 50%] Built target hello_library
/Applications/Xcode-beta.app/Contents/Developer/usr/bin/make  -f CMakeFiles/hello_binary.dir/build.make CMakeFiles/hello_binary.dir/depend
cd /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build && /opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E cmake_depends "Unix Makefiles" /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build/CMakeFiles/hello_binary.dir/DependInfo.cmake --color=
/Applications/Xcode-beta.app/Contents/Developer/usr/bin/make  -f CMakeFiles/hello_binary.dir/build.make CMakeFiles/hello_binary.dir/build
[ 75%] Building CXX object CMakeFiles/hello_binary.dir/src/main.cpp.o
/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++  -I/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/include -fuse-ld=lld -v -arch arm64 -isysroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -MD -MT CMakeFiles/hello_binary.dir/src/main.cpp.o -MF CMakeFiles/hello_binary.dir/src/main.cpp.o.d -o CMakeFiles/hello_binary.dir/src/main.cpp.o -c /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/src/main.cpp
clang version 15.0.0 (https://github.com/llvm/llvm-project/ 5bec1ea7a74895895e7831fd951dd8130d4f3d01)
Target: arm64-apple-darwin21.4.0
Thread model: posix
InstalledDir: /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin
clang: warning: argument unused during compilation: '-fuse-ld=lld' [-Wunused-command-line-argument]
 (in-process)
 "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang" -cc1 -triple arm64-apple-macosx12.0.0 -Wundef-prefix=TARGET_OS_ -Werror=undef-prefix -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cpp -mrelocation-model pic -pic-level 2 -mframe-pointer=non-leaf -ffp-contract=on -fno-rounding-math -funwind-tables=2 -target-sdk-version=12.3 -fcompatibility-qualified-id-block-type-checking -fvisibility-inlines-hidden-static-local-var -target-cpu apple-m1 -target-feature +v8.5a -target-feature +fp-armv8 -target-feature +neon -target-feature +crc -target-feature +crypto -target-feature +dotprod -target-feature +fp16fml -target-feature +ras -target-feature +lse -target-feature +rdm -target-feature +rcpc -target-feature +zcm -target-feature +zcz -target-feature +fullfp16 -target-feature +sha2 -target-feature +aes -target-abi darwinpcs -fallow-half-arguments-and-returns -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=lldb -target-linker-version 609.8 -v -fcoverage-compilation-dir=/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build -resource-dir /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0 -dependency-file CMakeFiles/hello_binary.dir/src/main.cpp.o.d -MT CMakeFiles/hello_binary.dir/src/main.cpp.o -sys-header-deps -isysroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -I /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/include -stdlib=libc++ -internal-isystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/../include/c++/v1 -internal-isystem /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/local/include -internal-isystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/include -internal-externc-isystem /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/include -fdeprecated-macro -fdebug-compilation-dir=/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build -ferror-limit 19 -stack-protector 1 -fblocks -fencode-extended-block-signature -fregister-global-dtors-with-atexit -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fmax-type-align=16 -fcolor-diagnostics -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/hello_binary.dir/src/main.cpp.o -x c++ /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/src/main.cpp
clang -cc1 version 15.0.0 based upon LLVM 15.0.0git default target x86_64-apple-darwin21.4.0
ignoring nonexistent directory "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/local/include"
ignoring nonexistent directory "/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/Library/Frameworks"
#include "..." search starts here:
#include <...> search starts here:
 /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/include
 /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/../include/c++/v1
 /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/include
 /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/include
 /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/System/Library/Frameworks (framework directory)
End of search list.
[100%] Linking CXX executable hello_binary
/opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E cmake_link_script CMakeFiles/hello_binary.dir/link.txt --verbose=1
/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/clang++  -fuse-ld=lld -v -arch arm64 -isysroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -Wl,-search_paths_first -Wl,-headerpad_max_install_names CMakeFiles/hello_binary.dir/src/main.cpp.o -o hello_binary  -Wl,-rpath,/Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build libhello_library.dylib 
clang version 15.0.0 (https://github.com/llvm/llvm-project/ 5bec1ea7a74895895e7831fd951dd8130d4f3d01)
Target: arm64-apple-darwin21.4.0
Thread model: posix
InstalledDir: /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin
 "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/ld64.lld" -demangle -dynamic -arch arm64 -platform_version macos 12.0.0 12.3 -syslibroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -o hello_binary -search_paths_first -headerpad_max_install_names CMakeFiles/hello_binary.dir/src/main.cpp.o -rpath /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build libhello_library.dylib -lc++ -lSystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/lib/darwin/libclang_rt.osx.a
[100%] Built target hello_binary
/opt/homebrew/Cellar/cmake/3.22.2/bin/cmake -E cmake_progress_start /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build/CMakeFiles 0
```

## linker

```shell
 "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/ld64.lld" -demangle -dynamic -dylib -arch arm64 -dylib_install_name @rpath/libhello_library.dylib -platform_version macos 12.0.0 12.3 -syslibroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -o libhello_library.dylib -headerpad_max_install_names CMakeFiles/hello_library.dir/src/Hello.cpp.o -lc++ -lSystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/lib/darwin/libclang_rt.osx.a
```

这是在对 `libhello_library.dylib` 进行动态链接，可以看到它会链接如下library：

1、`-lc++`

2、`-lSystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/lib/darwin/libclang_rt.osx.a`

```shell
 "/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/ld64.lld" -demangle -dynamic -arch arm64 -platform_version macos 12.0.0 12.3 -syslibroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -o hello_binary -search_paths_first -headerpad_max_install_names CMakeFiles/hello_binary.dir/src/main.cpp.o -rpath /Users/dengkai01/github/programming-language/docs/Build-system/CMake/CMake-toolchain/practice-use-chromium-toolchain/D-shared-library/build libhello_library.dylib -lc++ -lSystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/lib/darwin/libclang_rt.osx.a
```

这是在对 `hello_binary` 进行动态链接，可以看到它会链接如下library：

1、`libhello_library.dylib`

2、`-lc++`

3、`-lSystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/lib/darwin/libclang_rt.osx.a`

## 如何核实在编译过程中使用的linker？

### stackoverflow [Command to check the 'linker' used(by clang++) for executable creation on Mac OS](https://stackoverflow.com/questions/50246770/command-to-check-the-linker-usedby-clang-for-executable-creation-on-mac-os)

[A](https://stackoverflow.com/a/52031008)

Pass `-v` to see the subcommands clang runs. 

> NOTE:
>
> 从实践来看，加上 `-v` 后，能够看到具体的执行信息，上面就是具体的执行过程

Pass `-###` to only see the subcommands and make clang not actually run them.



## 替换标准库

将标准库替换为chromium的标准库。