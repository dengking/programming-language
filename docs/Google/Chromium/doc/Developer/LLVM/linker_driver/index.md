# `build/toolchain/apple/linker_driver.py`

是在单独编译`libc++`的时候，发现的这个script，下面是编译`libc++`的最后一步时候，执行的命令：


```shell
if [ ! -e "./libc++.dylib" -o ! -e "./libc++.dylib.TOC" ] || /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool -l "./libc++.dylib" | grep -q LC_REEXPORT_DYLIB; 
then 
TOOL_VERSION=1645592271 ../../build/toolchain/apple/linker_driver.py -Wcrl,strippath,/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip ../../third_party/llvm-build/Release+Asserts/bin/clang++ -shared -Wl,-install_name,@rpath/"libc++.dylib" -fuse-ld=lld -Wl,-fatal_warnings -Wl,--icf=all -Wl,--color-diagnostics -arch arm64 -no-canonical-prefixes -Wl,-dead_strip -nostdlib++ -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -Wl,-ObjC -Wl,-rpath,@loader_path/. -Wl,-rpath,@loader_path/../../.. -Werror -Wl,-U,_sanitizer_options_link_helper -o "./libc++.dylib" "@./libc++.dylib.rsp" && {
    /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool -l "./libc++.dylib" | grep LC_ID_DYLIB -A 5
    /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/nm -gPp "./libc++.dylib" | cut -f1-2 -d' ' | grep -v U$$
    true
} >"./libc++.dylib.TOC"; 
else 

TOOL_VERSION=1645592271 ../../build/toolchain/apple/linker_driver.py -Wcrl,strippath,/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip ../../third_party/llvm-build/Release+Asserts/bin/clang++ -shared -Wl,-install_name,@rpath/"libc++.dylib" -fuse-ld=lld -Wl,-fatal_warnings -Wl,--icf=all -Wl,--color-diagnostics -arch arm64 -no-canonical-prefixes -Wl,-dead_strip -nostdlib++ -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -Wl,-ObjC -Wl,-rpath,@loader_path/. -Wl,-rpath,@loader_path/../../.. -Werror -Wl,-U,_sanitizer_options_link_helper -o "./libc++.dylib" "@./libc++.dylib.rsp" && {
    /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool -l "./libc++.dylib" | grep LC_ID_DYLIB -A 5
    /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/nm -gPp "./libc++.dylib" | cut -f1-2 -d' ' | grep -v U$$
    true
} >"./libc++.dylib.tmp" && if ! cmp -s "./libc++.dylib.tmp" "./libc++.dylib.TOC"; then mv "./libc++.dylib.tmp" "./libc++.dylib.TOC"; fi; 


fi

```



显然上述命令是用于最终输出`libc++.dylib`。

## [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/main/build)/[toolchain](https://github.com/chromium/chromium/tree/main/build/toolchain)/[apple](https://github.com/chromium/chromium/tree/main/build/toolchain/apple)/[**linker_driver.py**](https://github.com/chromium/chromium/blob/main/build/toolchain/apple/linker_driver.py)

### doc

The `linker_driver.py` is responsible for forwarding a linker invocation to the **compiler driver**, **while** processing special arguments itself.

> NOTE: 
>
> 一、上述compiler driver指的是什么？这个问题后面会给出答案
>
> 二、上面这段话中**while**的意思是同时

Usage: 

```
linker_driver.py clang++ main.o -L. -llib -o prog -Wcrl,dsym,out
```

On Mac, the logical step of linking is handled by three discrete tools to perform the image link, debug info link, and strip. The `linker_driver.py` combines these three steps into a single tool.

The command passed to the `linker_driver.py` should be the **compiler driver invocation for the linker**. It is first invoked unaltered (except for the removal of the special driver arguments, described below). Then the driver performs additional actions, based on these arguments:

> NOTE:
>
> 一、"**compiler driver invocation for the linker** "指的是什么？
>
> 结合前面的例子来看，它指的是：
>
> ```shell
> ../../third_party/llvm-build/Release+Asserts/bin/clang++ -shared -Wl,-install_name,@rpath/"libc++.dylib" -fuse-ld=lld -Wl,-fatal_warnings -Wl,--icf=all -Wl,--color-diagnostics -arch arm64 -no-canonical-prefixes -Wl,-dead_strip -nostdlib++ -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -Wl,-ObjC -Wl,-rpath,@loader_path/. -Wl,-rpath,@loader_path/../../.. -Werror -Wl,-U,_sanitizer_options_link_helper -o "./libc++.dylib" "@./libc++.dylib.rsp" && {
>     /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool -l "./libc++.dylib" | grep LC_ID_DYLIB -A 5
>     /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/nm -gPp "./libc++.dylib" | cut -f1-2 -d' ' | grep -v U$$
>     true
> } >"./libc++.dylib.TOC"
> ```
>
> 



### `self._args`

```python
[
 '../../build/toolchain/apple/linker_driver.py', 
 '-Wcrl,strippath,/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip', 
 '../../third_party/llvm-build/Release+Asserts/bin/clang++', 
 '-shared', 
 '-Wl,-install_name,@rpath/libc++.dylib', 
 '-fuse-ld=lld', 
 '-Wl,-fatal_warnings', 
 '-Wl,--icf=all', 
 '-Wl,--color-diagnostics', 
 '-arch', 
 'arm64', 
 '-no-canonical-prefixes', 
 '-Wl,-dead_strip', 
 '-nostdlib++', 
 '-isysroot', '../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk', 
 '-mmacosx-version-min=10.11.0', 
 '-Wl,-ObjC', 
 '-Wl,-rpath,@loader_path/.', 
 '-Wl,-rpath,@loader_path/../../..', 
 '-Werror', 
 '-Wl,-U,_sanitizer_options_link_helper', 
 '-o', './libc++.dylib', 
 '@./libc++.dylib.rsp'
]
```

### `linker_driver_actions`

```python
{'strippath,': <function LinkerDriver._process_driver_arg.<locals>.<lambda> at 0x7f8388182040>}
```



### `compiler_driver_args`



```C++
['../../third_party/llvm-build/Release+Asserts/bin/clang++', '-shared', '-Wl,-install_name,@rpath/libc++.dylib', '-fuse-ld=lld', '-Wl,-fatal_warnings', '-Wl,--icf=all', '-Wl,--color-diagnostics', '-arch', 'arm64', '-no-canonical-prefixes', '-Wl,-dead_strip', '-nostdlib++', '-isysroot', '../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk', '-mmacosx-version-min=10.11.0', '-Wl,-ObjC', '-Wl,-rpath,@loader_path/.', '-Wl,-rpath,@loader_path/../../..', '-Werror', '-Wl,-U,_sanitizer_options_link_helper', '-o', './libc++.dylib', '@./libc++.dylib.rsp']
```



```shell
../../third_party/llvm-build/Release+Asserts/bin/clang++ -shared  -Wl,-install_name,@rpath/"libc++.dylib" -fuse-ld=lld -Wl,-fatal_warnings -Wl,--icf=all -Wl,--color-diagnostics -arch arm64 -no-canonical-prefixes -Wl,-dead_strip -nostdlib++ -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -Wl,-ObjC -Wl,-rpath,@loader_path/. -Wl,-rpath,@loader_path/../../.. -Werror -Wl,-U,_sanitizer_options_link_helper -o "./libc++.dylib" "@./libc++.dylib.rsp"
```

