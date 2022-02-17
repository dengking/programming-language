# [Chromium Style Plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) / [Chromium Style Checker Errors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/)

> NOTE: 
>
> 一、chromium不仅仅提出chromium C++ style guide，并且还基于clang开发了[Chromium Style Plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) ，用于检查代码是否遵循了style guide；

Due to constant over inlining, we now have plugins to the clang compiler which check for certain patterns that can increase code bloat. This document lists these errors, explains the motivation behind adding it as an error and shows how to fix them. The [chromium/clang wiki page](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/updating_clang_format_binaries.md) explains how you can run the plugin yourself, or how you can run your CLs through the clang trybots which run this plugin as well.

> NOTE: 
>
> 一、[Chromium Style Plugin](https://dev.chromium.org/developers/coding-style/chromium-style-checker-errors) 的目的主要是"check for certain patterns that can increase code bloat"

## [Constructor/Destructor Errors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#constructordestructor-errors)

> NOTE: 
>
> 非常值得一读



## [Virtual Method Out-of-lining](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#virtual-method-out-of-lining)

> NOTE: 
>
> 一、"out of line"的含义呢？
>
> "out of line"显然是和 "inline" 相对应的，在 stackoverflow [Out-of-Line Virtual Method](https://stackoverflow.com/questions/16801222/out-of-line-virtual-method) # [A](https://stackoverflow.com/a/16801276/10173843) 中有着非常好的解释:
>
> > The compiler must emit a vtable for classes with virtual methods. This contains the pointers to these methods. If all the virtual methods are inline (defined in the header), then the compiler doesn't know which translation unit (.cpp file) to emit the vtable within, so it emits a copy in all of them and the linker gets to deal with it. This makes extra work and bloat in the object files. If, on the other hand, a virtual function is defined out-of-line (in a .cpp), the vtable can be emitted there and thus only one copy will be emitted. The same applies to the RTTI.



## [Virtual methods and override/final](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#virtual-methods-and-overridefinal)



## [Redundant Virtual specifiers](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#redundant-virtual-specifiers)



## [Virtual final base class methods](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#virtual-final-base-class-methods)



## ["Using namespace" in a header](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#using-namespace-in-a-header)



## [RefCounted types with public destructors](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#refcounted-types-with-public-destructors)



## [Enumerator max values](https://www.chromium.org/developers/coding-style/chromium-style-checker-errors/#enumerator-max-values)