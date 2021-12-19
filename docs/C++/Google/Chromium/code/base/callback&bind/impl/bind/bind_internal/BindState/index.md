## `StorageTraits`

```
//  StorageTraits<> -- Type traits that determine how a bound argument is
//                     stored in BindState.
```

```C++
// StorageTraits<>
//
// See description at top of file.
template <typename T>
struct StorageTraits {
  using Type = T;
};

// For T*, store as UnretainedWrapper<T> for safety, as it internally uses
// raw_ptr<T> (when possible).
template <typename T>
struct StorageTraits<T*> {
  using Type = UnretainedWrapper<T>;
};

// Unwrap std::reference_wrapper and store it in a custom wrapper so that
// references are also protected with raw_ptr<T>.
template <typename T>
struct StorageTraits<std::reference_wrapper<T>> {
  using Type = UnretainedRefWrapper<T>;
};

template <typename T>
using MakeStorageType = typename StorageTraits<std::decay_t<T>>::Type;
```