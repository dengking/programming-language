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



## TODO

思考：什么地方将它挂载到了build graph上？

