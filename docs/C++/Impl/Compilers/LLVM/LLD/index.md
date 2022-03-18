# LLD

## LLD - The LLVM Linker[¶](https://lld.llvm.org/#lld-the-llvm-linker)

### Using LLD[¶](https://lld.llvm.org/#using-lld)

If you don’t want to change the system setting, you can use clang’s `-fuse-ld` option. In this way, you want to set `-fuse-ld=lld` to LDFLAGS when building your programs.

> NOTE:
>
> chromium就是这样用的，关于此，可以参见对应章节。
>
> 

## source code

[llvm-project](https://github.com/llvm/llvm-project)/[**lld**](https://github.com/llvm/llvm-project/tree/main/lld)/



## errors

### ld-link: error: invalid timestamp

```
ld-link: error: invalid timestamp: -2142000. Expected 32-bit integer
```

#### source code

[llvm-project](https://github.com/llvm/llvm-project)/[lld](https://github.com/llvm/llvm-project/tree/main/lld)/[COFF](https://github.com/llvm/llvm-project/tree/main/lld/COFF)/[**Driver.cpp#L1645**](https://github.com/llvm/llvm-project/blob/main/lld/COFF/Driver.cpp#L1645)

```c++
  // Handle /timestamp
  if (llvm::opt::Arg *arg = args.getLastArg(OPT_timestamp, OPT_repro)) {
    if (arg->getOption().getID() == OPT_repro) {
      config->timestamp = 0;
      config->repro = true;
    } else {
      config->repro = false;
      StringRef value(arg->getValue());
      if (value.getAsInteger(0, config->timestamp))
        fatal(Twine("invalid timestamp: ") + value +
              ".  Expected 32-bit integer");
    }
  } else {
    config->repro = false;
    config->timestamp = time(nullptr);
  }
```



reviews.llvm [**[lld] Use a real timestamp, with the option to use hash or explicit value**](https://reviews.llvm.org/D46966)

这篇文章讲述了添加上述code的原因



#### 如何解决

groups.google [Build different versions](https://groups.google.com/a/chromium.org/g/chromium-dev/c/FPwgrwOOsds)

```
What happens if you run `gn clean <out dir>` before your build?
```



## `Not an int attribute (Producer: '***' Reader: '***')`

碰到的报错如下：

```
warning: /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib: Not an int attribute (Producer: 'LLVM15.0.0git' Reader: 'LLVM APPLE_1_1316.0.20.6_0')
```

源代码位置：

https://llvm.org/doxygen/BitcodeReader_8cpp_source.html







### mac ranlib

#### unix man [ranlib(1) [osx man page]](https://www.unix.com/man-page/osx/1/ranlib/)



#### macosforge [Why ar doesn't call ranlib on Mac?](https://macports-users.macosforge.narkive.com/Wa9Zfepg/why-ar-doesn-t-call-ranlib-on-mac)

In building various Linux things from source on Mac, I've run into the "archive has no table of contents" problem several times. The fix is invariably(总是) to go and stick ranlib after each ar.

Now I wonder, why don't we replace `/usr/bin/ar` with a wrapper calling the `ar` and then `ranlib` if it's an ar c? No more stupid `ranlib` peppering then!

And what's wrong with Macs anyways? Why do we have to do it where Linux folks are happy without it?

#### the table of contents is empty

```
warning: /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib: archive library: lib/liblite.a the table of contents is empty (no object file members in the library define global symbols)

```

一、Google "ranlib has no symbols":

1、stackoverflow [What does "library has no symbol" mean? [duplicate]](https://stackoverflow.com/questions/44124219/what-does-library-has-no-symbol-mean)

2、stackoverflow [Building static libraries on Mac using CMake and GCC?](https://stackoverflow.com/questions/4929255/building-static-libraries-on-mac-using-cmake-and-gcc)

3、stackoverflow [Silence "table of contents is empty" warning](https://stackoverflow.com/questions/27602998/silence-table-of-contents-is-empty-warning)

二、Google "warning: /Applications/Xcode-beta.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib: archive library: lib/liblite.a the table of contents is empty (no object file members in the library define global symbols)"

1、stackoverflow [iOS Static library compilation error - table of contents is empty](https://stackoverflow.com/questions/24750156/ios-static-library-compilation-error-table-of-contents-is-empty)

2、github [clang warnings macOS #3331](https://github.com/widelands/widelands/issues/3331)

3、stackoverflow [Static library link issue with Mac OS X: symbol(s) not found for architecture x86_64](https://stackoverflow.com/questions/30948807/static-library-link-issue-with-mac-os-x-symbols-not-found-for-architecture-x8)



### github [[BUG] ld.lld: error: Invalid record (Producer: 'LLVM12.0.5git' Reader: 'LLVM 12.0.5git') #1591](https://github.com/android/ndk/issues/1591)

#### Description

Happens when LTO is enabled for armeabi-v7a ABI with NDK r23 - the same code works correctly with NDK r22b.

I still haven't been able to create a minimum reproducible sample (I'll update this issue if I manage to do so), but [@lmglmg](https://github.com/lmglmg) and I observed the same issue also with `armv7` slice when building the same code for iOS using Xcode 13 (it also uses the LLVM 12-based backend).

I'm posting this here in case if someone has already seen this issue and found a workaround.

#### Environment Details

- NDK Version: r23
- Build system: cmake
- Host OS: MacOS
- ABI: armeabi-v7a
- NDK API level: 16
- Device API level: 16







### github [Does not build with LLVM/clang/lld #45](https://github.com/libpwquality/libpwquality/issues/45)



```
/Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/bin/ld64.lld -all_load -demangle -dynamic -dylib -arch x86_64 -dylib_install_name @rpath/nertc_sdk_Mac.framework/Versions/A/nertc_sdk_Mac -platform_version macos 10.11.0 12.3 -syslibroot /Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk -ObjC -o ../../../../../../lib/mac/nertc_sdk_Mac.framework/Versions/A/nertc_sdk_Mac -L/Users/dengkai01/netease/lava/nertcsdk/build/mac/out/x86_64/Release/lib -L/Users/dengkai01/netease/lava/nertcsdk/../lib/mac -headerpad_max_install_names -dead_strip nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_engine_factory.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_engine_interface_impl.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine_audio.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine_audio_mix.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine_live.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine_screencapture.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine_sink.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine_video.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/nertc_internal_engine_widget.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/util/nertc_util.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/util/string_util.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/device/nertc_audio_device_manager_impl.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/device/nertc_device_info.cc.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/device/nertc_video_device_manager_impl.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/observer/nertc_audio_frame_observer.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/observer/nertc_media_stats_observer.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/channel/nertc_channel_bridge_sink.cpp.o nertcsdk/src/sdk/pc/CMakeFiles/nertc_sdk_Mac.dir/src/channel/nertc_channel_impl.cpp.o -rpath /Users/dengkai01/netease/lava/nertcsdk/build/mac/out/x86_64/Release/lib -rpath /Users/dengkai01/netease/lava/nertcsdk/../lib/mac lib/liblite.a lib/liblava.a -framework MetalPerformanceShaders -framework VideoToolbox -framework QuartzCore -framework Security -framework CoreVideo -framework AudioToolbox -framework Metal -framework MetalKit -framework CoreAudio -framework CoreMedia -framework OpenGL -framework AVFoundation -framework AppKit -framework Foundation -framework CoreServices -framework SystemConfiguration -framework GLKit -framework IOSurface -framework IOKit -lprotoopp -lwebrtc -lmediasoupclient -lsdptransform -lne264 -lNE265 -lnenn -framework Foundation -lobjc -lc++ -lSystem /Users/dengkai01/chromium/src/third_party/llvm-build/Release+Asserts/lib/clang/15.0.0/lib/darwin/libclang_rt.osx.a
```

