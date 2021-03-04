# Span-for-multidimensional-array



## stackoverflow [CPPCoreGuidelines span for a T** interface?](https://stackoverflow.com/questions/41880770/cppcoreguidelines-spant-for-a-t-interface)

In digital signal processing audio is commonly passed around as a 2D array of channels and samples i.e.

```cpp
void use(float** buffer, int channels, int samples) { ... }
```