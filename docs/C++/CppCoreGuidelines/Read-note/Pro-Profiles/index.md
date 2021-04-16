# CppCoreGuidelines [Pro: Profiles](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#pro-profiles)

Profiles summary:

- [Pro.type: Type safety](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-type)
- [Pro.bounds: Bounds safety](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-bounds)
- [Pro.lifetime: Lifetime safety](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-lifetime)

In the future, we expect to define many more profiles and add more checks to existing profiles. Candidates include:

- narrowing arithmetic promotions/conversions (likely part of a separate safe-arithmetic profile)
- arithmetic cast from negative floating point to unsigned integral type (ditto)
- selected undefined behavior: Start with Gabriel Dos Reis's UB list developed for the WG21 study group
- selected unspecified behavior: Addressing portability concerns.
- `const` violations: Mostly done by compilers already, but we can catch inappropriate casting and underuse of `const`.

Enabling a profile is implementation defined; typically, it is set in the analysis tool used.

To suppress enforcement of a profile check, place a `suppress` annotation on a language contract. For example:

```C++
[[suppress(bounds)]] char* raw_find(char* p, int n, char x)    // find x in p[0]..p[n - 1]
{
    // ...
}
```

Now `raw_find()` can scramble memory to its heart's content. Obviously, suppression should be very rare.



## [Pro.bounds: Bounds safety profile](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#probounds-bounds-safety-profile)