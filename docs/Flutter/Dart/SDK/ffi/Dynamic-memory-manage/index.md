# Dart dynamic memory management

在 [dart:ffi library](https://api.dart.cn/stable/2.13.4/dart-ffi/dart-ffi-library.html) 中，如下的class是用于dynamic memory management的: 

## interface

官方文档的 [Allocator class](https://api.dart.cn/stable/2.13.4/dart-ffi/Allocator-class.html) 是interface

doc: [Allocator class](https://api.dart.cn/stable/2.13.4/dart-ffi/Allocator-class.html)

code: github [sdk](https://github.com/dart-lang/sdk)/[sdk](https://github.com/dart-lang/sdk/tree/main/sdk)/[lib](https://github.com/dart-lang/sdk/tree/main/sdk/lib)/[ffi](https://github.com/dart-lang/sdk/tree/main/sdk/lib/ffi)/**[allocation.dart](https://github.com/dart-lang/sdk/blob/main/sdk/lib/ffi/allocation.dart)**

## implementation

在 github [dart-lang](https://github.com/dart-lang)/**[ffi](https://github.com/dart-lang/ffi)** 中，给出了implementation，具体参见如下code:

https://github.com/dart-lang/ffi/tree/master/lib



https://github.com/dart-lang/ffi/blob/master/lib/src/allocation.dart

```dart
class _MallocAllocator implements Allocator
```



```dart
const Allocator malloc = _MallocAllocator();
```



## example

### 1

```dart
class Tag_CApiNIM_AuthInfo extends ffi.Struct {
  external ffi.Pointer<ffi.Int8> appKey;

  external ffi.Pointer<ffi.Int8> accid;

  external ffi.Pointer<ffi.Int8> token;
}

account1 = malloc<nim.Tag_CApiNIM_AuthInfo>();
account1.ref.appKey = '45c6af3c98409b18a84451215d0bdd6e'.toNativeUtf8().cast();
account1.ref.accid = 'cqu227hk'.toNativeUtf8().cast();
account1.ref.token = 'a38355b1941984c6e97de6e228717986'.toNativeUtf8().cast();

account2 = malloc<nim.Tag_CApiNIM_AuthInfo>();
account2.ref.appKey = '45c6af3c98409b18a84451215d0bdd6e'.toNativeUtf8().cast();
account2.ref.accid = 'cqu227hk2'.toNativeUtf8().cast();
account2.ref.token = 'a38355b1941984c6e97de6e228717986'.toNativeUtf8().cast();

malloc.free(account1);
malloc.free(account2); // 释放分配的内存
```

### 2

```dart
    /// 将字符串传入到C中
    if (linkAddrs.isNotEmpty) {
      cStruct.ref.linkAddrs.data = malloc
          .allocate<Pointer<Int8>>(sizeOf<Pointer<Int8>>() * linkAddrs.length);
      cStruct.ref.linkAddrs.len = linkAddrs.length;
      for (var i = 0; i < linkAddrs.length; ++i) {
        cStruct.ref.linkAddrs.data[i] = linkAddrs[i].toNativeUtf8().cast();
      }
    }
```



## dart.ffi `sizeof`

### api.dart.dev [`sizeOf<T extends NativeType>` function](https://api.dart.dev/stable/2.14.4/dart-ffi/sizeOf.html)





