# chromium build target



## 最终target

```shell
[50880/50948] ../../third_party/llvm-build/Release+Asserts/bin/clang++ -MMD -MF obj/chrome/chrome_app_executable/chrome_exe_main_mac.o.d -DDCHECK_ALWAYS_ON=1 -D_LIBCPP_HAS_NO_ALIGNED_ALLOCATION -DCR_XCODE_VERSION=1330 -DCR_CLANG_REVISION=\"llvmorg-15-init-1995-g5bec1ea7-1\" -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D_FORTIFY_SOURCE=2 -DCOMPONENT_BUILD -D_LIBCPP_ABI_UNSTABLE -D_LIBCPP_ABI_VERSION=Cr -D_LIBCPP_ENABLE_NODISCARD -D_LIBCPP_DEBUG=0 -DCR_LIBCXX_REVISION=79a2e924d96e2fc1e4b937c42efd08898fa472d7 -D__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES=0 -DNDEBUG -DNVALGRIND -DDYNAMIC_ANNOTATIONS_ENABLED=0 -DWEBP_EXTERN=extern -DVK_USE_PLATFORM_METAL_EXT -DUSE_EGL -D_WTL_NO_AUTOMATIC_NAMESPACE -DTOOLKIT_VIEWS=1 -DON_FOCUS_PING_ENABLED -I../.. -Igen -I../../buildtools/third_party/libc++ -I../../third_party/perfetto/include -Igen/third_party/perfetto/build_config -Igen/third_party/perfetto -I../../third_party/libwebp/src -I../../third_party/libyuv/include -I../../third_party/jsoncpp/source/include -I../../third_party/khronos -I../../gpu -I../../third_party/vulkan-deps/vulkan-headers/src/include -Igen/third_party/dawn/include -I../../third_party/dawn/include -I../../third_party/dawn/src/include -I../../third_party/wtl/include -fno-delete-null-pointer-checks -fno-ident -fno-strict-aliasing -fstack-protector -fcolor-diagnostics -fmerge-all-constants -fcrash-diagnostics-dir=../../tools/clang/crashreports -mllvm -instcombine-lower-dbg-declare=0 -ffp-contract=off -fcomplete-member-pointers -arch arm64 -Wno-builtin-macro-redefined -D__DATE__= -D__TIME__= -D__TIMESTAMP__= -ffile-compilation-dir=. -no-canonical-prefixes -Wall -Werror -Wextra -Wimplicit-fallthrough -Wunreachable-code-aggressive -Wthread-safety -Wextra-semi -Wunguarded-availability -Wno-missing-field-initializers -Wno-unused-parameter -Wloop-analysis -Wno-unneeded-internal-declaration -Wenum-compare-conditional -Wno-psabi -Wno-ignored-pragma-optimize -Wshadow -O2 -fno-omit-frame-pointer -g0 -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -ftrivial-auto-var-init=pattern -fvisibility=hidden -Xclang -add-plugin -Xclang find-bad-constructs -Xclang -plugin-arg-find-bad-constructs -Xclang raw-ptr-template-as-trivial-member -Xclang -plugin-arg-find-bad-constructs -Xclang use-classify-type -Wheader-hygiene -Wstring-conversion -Wtautological-overlap-compare -Wexit-time-destructors -std=c++17 -Wno-trigraphs -fno-aligned-new -fno-exceptions -fno-rtti -nostdinc++ -isystem../../buildtools/third_party/libc++/trunk/include -isystem../../buildtools/third_party/libc++abi/trunk/include -fvisibility-inlines-hidden -Wno-deprecated-declarations -c ../../chrome/app/chrome_exe_main_mac.cc -o obj/chrome/chrome_app_executable/chrome_exe_main_mac.o

```





```shell
[50881/50948] TOOL_VERSION=1645592271 ../../build/toolchain/apple/linker_driver.py -Wcrl,strippath,/Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip ../../third_party/llvm-build/Release+Asserts/bin/clang++  -Wl,-rpath,@executable_path/../Frameworks -fuse-ld=lld -Wl,-fatal_warnings -Wl,--icf=all -Wl,--color-diagnostics -arch arm64 -no-canonical-prefixes -Werror -Wl,-dead_strip -nostdlib++ -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -Wl,-ObjC -Wl,-rpath,@loader_path/. -Wl,-rpath,@loader_path/../../.. -F. -o "obj/chrome/chrome_app_executable/Chromium" "@obj/chrome/chrome_app_executable/Chromium.rsp"

```

从 [Checking out and building Chromium for Mac](https://chromium.googlesource.com/chromium/src/+/main/docs/mac_build_instructions.md) # [Run Chromium](https://chromium.googlesource.com/chromium/src/+/main/docs/mac_build_instructions.md#Run-Chromium) 可知，最终的产物是:

```shell
$ out/Default/Chromium.app/Contents/MacOS/Chromium
```

因此可以初步断定，上述command就是生成最终产物的。