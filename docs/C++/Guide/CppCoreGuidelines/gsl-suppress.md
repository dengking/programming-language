# gsl suppress



## [isocpp](https://github.com/isocpp)/**[CppCoreGuidelines](https://github.com/isocpp/CppCoreGuidelines)** # [Suppression guidelines #1173](https://github.com/isocpp/CppCoreGuidelines/issues/1173)

### **[annagrin](https://github.com/annagrin)** commented [on 22 Mar 2018](https://github.com/isocpp/CppCoreGuidelines/issues/1173#issue-307498340)

GSL library uses attributes like `[[gsl::suppress(type.1)]]` to suppress warnings.

Different compilers require different syntax (clang) or do not recognize them (gcc). This means that using suppress in GSL itself is complicated - not every static analysis tool will be able to suppress them, and we need ifdefs around suppressions to make sure they even compile.

Currently `[[gsl::suppress]]` are only recognized by CppCoreCheck. not compilers, which could have some warnings from Cpp Core Guidelines as well (for example, unitialized variables)

Would be great to create a standard suppression mechanism for compiler and static analysis warnings.

### **[hsutter](https://github.com/hsutter)** commented [on 27 Mar 2018](https://github.com/isocpp/CppCoreGuidelines/issues/1173#issuecomment-376265886)

Thanks Anna, some more context Anna provided separately:

MSVC understands `[[gsl::suppress(something.x)]]`
Clang understands `[[gsl::suppress(“something.x”)]]`
GCC doesn't understand them on a line inside a function



## [microsoft](https://github.com/microsoft)/[include](https://github.com/microsoft/GSL/tree/main/include)/[gsl](https://github.com/microsoft/GSL/tree/main/include/gsl)/[assert](https://github.com/microsoft/GSL/blob/main/include/gsl/assert) `GSL_SUPPRESS`

```C++
//
// make suppress attributes parse for some compilers
// Hopefully temporary until suppression standardization occurs
//
#if defined(__clang__)
#define GSL_SUPPRESS(x) [[gsl::suppress("x")]]
#else
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define GSL_SUPPRESS(x) [[gsl::suppress(x)]]
#else
#define GSL_SUPPRESS(x)
#endif // _MSC_VER
#endif // __clang__
```

