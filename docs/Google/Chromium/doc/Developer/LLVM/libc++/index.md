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

## 如何验证使用的是哪个linker？

使用的linker应该是与`../../third_party/llvm-build/Release+Asserts/bin/clang++`位于同一个目录下的`lld`，拿如何来验证呢？验证方法非常简单，将上述文件改下名字，然后重新运行上述流程，结果报错如下：

```shell
FAILED: libc++.dylib libc++.dylib.TOC 
if [ ! -e "./libc++.dylib" -o ! -e "./libc++.dylib.TOC" ] || /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool -l "./libc++.dylib" | grep -q LC_REEXPORT_DYLIB ; then TOOL_VERSION=1646984320 ../../build/toolchain/apple/linker_driver.py -Wcrl,strippath,/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip ../../third_party/llvm-build/Release+Asserts/bin/clang++ -shared  -Wl,-install_name,@rpath/"libc++.dylib" -fuse-ld=lld -Wl,-fatal_warnings -Wl,--icf=all -Wl,--color-diagnostics -arch arm64 -no-canonical-prefixes -Wl,-dead_strip -nostdlib++ -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -Wl,-ObjC -Wl,-rpath,@loader_path/. -Wl,-rpath,@loader_path/../../.. -Werror -Wl,-U,_sanitizer_options_link_helper -o "./libc++.dylib" "@./libc++.dylib.rsp" && { /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool -l "./libc++.dylib" | grep LC_ID_DYLIB -A 5; /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/nm -gPp "./libc++.dylib" | cut -f1-2 -d' ' | grep -v U$$; true; } > "./libc++.dylib.TOC"; else TOOL_VERSION=1646984320 ../../build/toolchain/apple/linker_driver.py -Wcrl,strippath,/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip ../../third_party/llvm-build/Release+Asserts/bin/clang++ -shared  -Wl,-install_name,@rpath/"libc++.dylib" -fuse-ld=lld -Wl,-fatal_warnings -Wl,--icf=all -Wl,--color-diagnostics -arch arm64 -no-canonical-prefixes -Wl,-dead_strip -nostdlib++ -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -Wl,-ObjC -Wl,-rpath,@loader_path/. -Wl,-rpath,@loader_path/../../.. -Werror -Wl,-U,_sanitizer_options_link_helper -o "./libc++.dylib" "@./libc++.dylib.rsp" && { /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/otool -l "./libc++.dylib" | grep LC_ID_DYLIB -A 5; /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/nm -gPp "./libc++.dylib" | cut -f1-2 -d' ' | grep -v U$$; true; } > "./libc++.dylib.tmp" && if ! cmp -s "./libc++.dylib.tmp" "./libc++.dylib.TOC"; then mv "./libc++.dylib.tmp" "./libc++.dylib.TOC" ; fi; fi
clang++: error: invalid linker name in argument '-fuse-ld=lld'
Traceback (most recent call last):
  File "/Users/dengkai01/chromium/src/out/Default/../../build/toolchain/apple/linker_driver.py", line 341, in <module>
    LinkerDriver(sys.argv).run()
  File "/Users/dengkai01/chromium/src/out/Default/../../build/toolchain/apple/linker_driver.py", line 117, in run
    subprocess.check_call(compiler_driver_args, env=env)
  File "/Users/dengkai01/opt/anaconda3/lib/python3.9/subprocess.py", line 373, in check_call
    raise CalledProcessError(retcode, cmd)
subprocess.CalledProcessError: Command '['../../third_party/llvm-build/Release+Asserts/bin/clang++', '-shared', '-Wl,-install_name,@rpath/libc++.dylib', '-fuse-ld=lld', '-Wl,-fatal_warnings', '-Wl,--icf=all', '-Wl,--color-diagnostics', '-arch', 'arm64', '-no-canonical-prefixes', '-Wl,-dead_strip', '-nostdlib++', '-isysroot', '../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk', '-mmacosx-version-min=10.11.0', '-Wl,-ObjC', '-Wl,-rpath,@loader_path/.', '-Wl,-rpath,@loader_path/../../..', '-Werror', '-Wl,-U,_sanitizer_options_link_helper', '-o', './libc++.dylib', '@./libc++.dylib.rsp']' returned non-zero exit status 1.
ninja: build stopped: subcommand failed.

```

显然，通过上述过程就知道了它使用的linker与前面的猜测是一致的。





## TOC file

[*.so.TOC files](https://groups.google.com/a/chromium.org/g/chromium-discuss/c/vBMQzOo74rY)

## TODO

思考：什么地方将它挂载到了build graph上？

