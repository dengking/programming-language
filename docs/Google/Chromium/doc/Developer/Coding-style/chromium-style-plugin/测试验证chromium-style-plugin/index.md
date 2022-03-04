# 测试验证chromium-style-plugin

按照chromium官方文档

1、 [Chromium Style Checker Errors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/) 

2、[Clang](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/clang.md)

中的说明，它能够在编译时判定那些可能会导致代码膨胀的一些模式，那如何来验证呢？下面是我的验证过程：

 **Source code**

下面是按照  [Chromium Style Checker Errors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/) 中给出的样例写的一个测试程序，它故意让成员尽可能多，并且使用compile defaulted default  constructor，并且将constructor放到了header file中

```c++
#include <map>
#include <string>
#include <vector>
struct BigStruct {
  double f[10000];
  double f2[10000000];
  long long f3;
  double f4;
  double f5[10000000];
  double f6[10000000];
  double f7[10000000];
};


class BigObject {
 public:
  BigObject() {
    for (int i = 0; i < 10; ++i) {
      f1.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f2.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f3.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f4.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f5.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f7.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f7.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f8.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f9.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f10.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f11.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f12.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f13.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f14.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f14.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f15.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f16.push_back(BigStruct());
    }
    for (int i = 0; i < 10; ++i) {
      f17.push_back(BigStruct());
    }
  }


 private:
  std::vector<BigStruct> f1;
  std::vector<BigStruct> f2;
  std::vector<BigStruct> f3;
  std::vector<BigStruct> f4;
  std::vector<BigStruct> f5;
  std::vector<BigStruct> f6;
  std::vector<BigStruct> f7;
  std::vector<BigStruct> f8;
  std::vector<BigStruct> f9;
  std::vector<BigStruct> f10;
  std::vector<BigStruct> f11;
  std::vector<BigStruct> f12;
  std::vector<BigStruct> f13;
  std::vector<BigStruct> f14;
  std::vector<BigStruct> f15;
  std::vector<BigStruct> f16;
  std::vector<BigStruct> f17;
};


class ComplexStuff {
 public:
  void NoDeclaredConstructor() {}


 private:
  // Enough data members to trip the detector


  BigObject big_object_;
  BigObject big_object_1;
  BigObject big_object_2;
  BigObject big_object_3;
  BigObject big_object_4;
  BigObject big_object_6;
  BigObject big_object_7;
  BigObject big_object_8;
  BigObject big_object_9;
  BigObject big_object_10;
  std::map<std::string, BigObject> complex_stl_stuff_;
};
```



下面是驱动程序:

```c++
#include "test.h"
int main() {
  ComplexStuff c;
  c.NoDeclaredConstructor();
}
```



编译指令如下:

```shell
../../third_party/llvm-build/Release+Asserts/bin/clang++ -MMD -MF obj/apps/apps/app_lifetime_monitor.o.d -DDCHECK_ALWAYS_ON=1 -D_LIBCPP_HAS_NO_ALIGNED_ALLOCATION -DCR_XCODE_VERSION=1330 -DCR_CLANG_REVISION=\\\"llvmorg-15-init-1995-g5bec1ea7-1\\\" -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D_FORTIFY_SOURCE=2 -DCOMPONENT_BUILD -D_LIBCPP_ABI_UNSTABLE -D_LIBCPP_ABI_VERSION=Cr -D_LIBCPP_ENABLE_NODISCARD -D_LIBCPP_DEBUG=0 -DCR_LIBCXX_REVISION=79a2e924d96e2fc1e4b937c42efd08898fa472d7 -D__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES=0 -DNDEBUG -DNVALGRIND -DDYNAMIC_ANNOTATIONS_ENABLED=0 -DWEBP_EXTERN=extern -DVK_USE_PLATFORM_METAL_EXT -DUSE_EGL -D_WTL_NO_AUTOMATIC_NAMESPACE -DABSL_CONSUME_DLL -DBORINGSSL_SHARED_LIBRARY -DGOOGLE_PROTOBUF_NO_RTTI -DGOOGLE_PROTOBUF_NO_STATIC_INITIALIZER -DHAVE_PTHREAD -DPROTOBUF_USE_DLLS -DU_USING_ICU_NAMESPACE=0 -DU_ENABLE_DYLOAD=0 -DUSE_CHROMIUM_ICU=1 -DU_ENABLE_TRACING=1 -DU_ENABLE_RESOURCE_TRACING=0 -DICU_UTIL_DATA_IMPL=ICU_UTIL_DATA_FILE -DWEBRTC_ENABLE_SYMBOL_EXPORT -DWEBRTC_ENABLE_AVX2 -DWEBRTC_NON_STATIC_TRACE_EVENT_HANDLERS=0 -DWEBRTC_CHROMIUM_BUILD -DWEBRTC_POSIX -DWEBRTC_MAC -DABSL_ALLOCATOR_NOTHROW=1 -DWEBRTC_USE_BUILTIN_ISAC_FIX=0 -DWEBRTC_USE_BUILTIN_ISAC_FLOAT=1 -DWEBRTC_HAVE_SCTP -DLOGGING_INSIDE_WEBRTC -DSK_CODEC_DECODES_PNG -DSK_CODEC_DECODES_WEBP -DSK_ENCODE_PNG -DSK_ENCODE_WEBP -DSK_ENABLE_SKSL -DSK_UNTIL_CRBUG_1187654_IS_FIXED -DSK_USER_CONFIG_HEADER=\\\"../../skia/config/SkUserConfig.h\\\" -DSK_WIN_FONTMGR_NO_SIMULATIONS -DSK_GL -DSK_CODEC_DECODES_JPEG -DSK_ENCODE_JPEG -DSK_HAS_WUFFS_LIBRARY -DSK_VULKAN=1 -DSKIA_DLL -DSK_SUPPORT_GPU=1 -DSK_GPU_WORKAROUNDS_HEADER=\\\"gpu/config/gpu_driver_bug_workaround_autogen.h\\\" -DSK_BUILD_FOR_MAC -DSK_METAL -DLEVELDB_PLATFORM_CHROMIUM=1 -DLEVELDB_SHARED_LIBRARY -DV8_USE_EXTERNAL_STARTUP_DATA -DUSE_V8_CONTEXT_SNAPSHOT -DV8_CONTEXT_SNAPSHOT_FILENAME=\\\"v8_context_snapshot.arm64.bin\\\" -DUSING_V8_SHARED -DV8_COMPRESS_POINTERS -DV8_COMPRESS_POINTERS_IN_SHARED_CAGE -DV8_31BIT_SMIS_ON_64BIT_ARCH -DV8_SANDBOX -DV8_DEPRECATION_WARNINGS -DCPPGC_CAGED_HEAP -DFLATBUFFERS_LOCALE_INDEPENDENT=0 -I../.. -Igen -I../../buildtools/third_party/libc++ -I../../third_party/perfetto/include -Igen/third_party/perfetto/build_config -Igen/third_party/perfetto -I../../third_party/khronos -I../../gpu -I../../third_party/libwebp/src -I../../third_party/vulkan-deps/vulkan-headers/src/include -Igen/third_party/dawn/include -I../../third_party/dawn/include -I../../third_party/dawn/src/include -I../../third_party/libyuv/include -I../../third_party/jsoncpp/source/include -I../../third_party/wtl/include -I../../third_party/abseil-cpp -I../../third_party/boringssl/src/include -I../../third_party/protobuf/src -Igen/protoc_out -Igen/net/third_party/quiche/src -I../../net/third_party/quiche/overrides -I../../net/third_party/quiche/src/common/platform/default -I../../net/third_party/quiche/src -I../../third_party/ced/src -I../../third_party/icu/source/common -I../../third_party/icu/source/i18n -I../../third_party/webrtc_overrides -I../../third_party/webrtc -Igen/third_party/webrtc -I../../third_party/skia -I../../third_party/wuffs/src/release/c -I../../third_party/vulkan/include -I../../third_party/mesa_headers -I../../third_party/libwebm/source -I../../third_party/leveldatabase -I../../third_party/leveldatabase/src -I../../third_party/leveldatabase/src/include -I../../third_party/libaom/source/libaom -I../../v8/include -Igen/v8/include -I../../third_party/re2/src -I../../third_party/flatbuffers/src/include -fno-delete-null-pointer-checks -fno-ident -fno-strict-aliasing -fstack-protector -fcolor-diagnostics -fmerge-all-constants -fcrash-diagnostics-dir=../../tools/clang/crashreports -mllvm -instcombine-lower-dbg-declare=0 -ffp-contract=off -fcomplete-member-pointers -arch arm64 -Wno-builtin-macro-redefined -D__DATE__= -D__TIME__= -D__TIMESTAMP__= -ffile-compilation-dir=. -no-canonical-prefixes -Wall -Werror -Wextra -Wimplicit-fallthrough -Wunreachable-code-aggressive -Wthread-safety -Wextra-semi -Wunguarded-availability -Wno-missing-field-initializers -Wno-unused-parameter -Wloop-analysis -Wno-unneeded-internal-declaration -Wenum-compare-conditional -Wno-psabi -Wno-ignored-pragma-optimize -Wshadow -O2 -fno-omit-frame-pointer -g0 -isysroot ../../../../../../Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -mmacosx-version-min=10.11.0 -ftrivial-auto-var-init=pattern -fvisibility=hidden -Xclang -add-plugin -Xclang find-bad-constructs -Xclang -plugin-arg-find-bad-constructs -Xclang raw-ptr-template-as-trivial-member -Xclang -plugin-arg-find-bad-constructs -Xclang use-classify-type -Wheader-hygiene -Wstring-conversion -Wtautological-overlap-compare -Wexit-time-destructors -DPROTOBUF_ALLOW_DEPRECATED=1 -Wno-shadow -std=c++17 -Wno-trigraphs -fno-aligned-new -fno-exceptions -fno-rtti -nostdinc++ -isystem../../buildtools/third_party/libc++/trunk/include -isystem../../buildtools/third_party/libc++abi/trunk/include -fvisibility-inlines-hidden -Wno-deprecated-declarations -c test.cpp -o ./test.o

```



编译报错如下:

```c++
In file included from test.cpp:1:
./test.h:16:3: error: [chromium-style] Complex constructor has an inlined body.
  BigObject() {
  ^
./test.h:14:1: error: [chromium-style] Complex class/struct needs an explicit out-of-line destructor.
class BigObject {
^
./test.h:93:1: error: [chromium-style] Complex class/struct needs an explicit out-of-line constructor.
class ComplexStuff {
^
./test.h:93:1: error: [chromium-style] Complex class/struct needs an explicit out-of-line destructor.
8 errors generated.

```



显然上述报错就是chromium-style-plugin生效了。

**结论**

1、chromium的clang程序中，以及包含了chromium-style-plugin，并且chromium-style-plugin并没有按照 [Clang Plugins](https://clang.llvm.org/docs/ClangPlugins.html) 中所说的dynamic library中方式，而是直接编译到了clang中

2、FindBadConstructs plugin应该就是chromium style plugin