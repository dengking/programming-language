# `dart:ffi`

## dart implementation and `dart::ffi` 

一、dart是GC、完全OOP的language，在dart中，无法获取一个dart object的memory address，dart语言层就拒绝了这种操作，这是因为:

1、stackoverflow [How can I get the memory address of a Flutter / Dart variable? Is this possible?](https://stackoverflow.com/questions/59718851/how-can-i-get-the-memory-address-of-a-flutter-dart-variable-is-this-possible) # [A](https://stackoverflow.com/a/59726800/10173843)

> It's not possible to access raw memory of Dart objects. Dart is a garbage collected language which means that Dart objects are not guaranteed to live at a particular memory address as the garbage collector can (and *certainly will*) move these objects to different memory locations during a garbage collection. Within the Dart virtual machine Dart objects are almost exclusively accessed and passed around via handles and not raw pointers for this very reason.

> NOTE: 上述解释是非常到位的



2、stackoverflow [How can I get the memory address of a Flutter / Dart variable? Is this possible?](https://stackoverflow.com/questions/59718851/how-can-i-get-the-memory-address-of-a-flutter-dart-variable-is-this-possible) # [A](https://stackoverflow.com/a/59729176/10173843)

> There is no way to print the memory address of a variable in Dart.
>
> Even if you could print the memory address, there is nothing meaningful that you could do with it.
>
> If you want to identify objects, you could use [`identityHashCode`](https://api.dart.dev/stable/2.5.1/dart-core/identityHashCode.html).



3、由于无法获得一个dart object的address，因此，将一个dart object转化为 `ffi::Pointer<void>` 是无法实现的

二、dart object identifier

1、stackoverflow [flutter dart how to get object id](https://stackoverflow.com/questions/62574124/flutter-dart-how-to-get-object-id) # [A](https://stackoverflow.com/a/65063297/10173843)

> With this function [`identityHashCode`](https://api.dart.dev/stable/2.5.1/dart-core/identityHashCode.html).
>
> It return unique id (int) of object.



2、api.dart [identityHashCode function](https://api.dart.dev/stable/2.5.1/dart-core/identityHashCode.html)

三、`dart::ffi` 中提供了:

1、dynamic memory allocation

返回值类型是 `ffi::Pointer` 

通过查看`dart::ffi`的 dynamic memory allocation的implementation(github [sdk](https://github.com/dart-lang/sdk)/[sdk](https://github.com/dart-lang/sdk/tree/main/sdk)/[lib](https://github.com/dart-lang/sdk/tree/main/sdk/lib)/[ffi](https://github.com/dart-lang/sdk/tree/main/sdk/lib/ffi)/**[allocation.dart](https://github.com/dart-lang/sdk/blob/main/sdk/lib/ffi/allocation.dart)** 、github [dart-lang](https://github.com/dart-lang)/**[ffi](https://github.com/dart-lang/ffi)**/src/[allocation.dart](https://github.com/dart-lang/ffi/blob/master/lib/src/allocation.dart) )可知: 

```dart
Pointer<T> allocate<T extends NativeType>(int byteCount, {int? alignment});
```

它要求模板参数为 `T extends NativeType`。

因此，在dart中，programmer只能够获得dynamic memory allocation的object的address，并且这些object只能够是 `NativeType`。

2、`ffi::Pointer`



3、`dart::ffi` 提供了如下function:

```dart
Pointer<T extends NativeType>::fromFunction<T extends Function>
```

这个函数的入参不能够是member method，可以是global function、static method；

这说明dart是支持获取非dynamic method的address的，这是因为: dart语言的function的address是能够在编译时确定的，不会再运行时改编，因此是可以拿到它的address的。



四、stackoverflow [Pass Uint8List to Pointer in dart:ffi](https://stackoverflow.com/questions/58838193/pass-uint8list-to-pointervoid-in-dartffi)



## Example



### 官方例子

**sqlite**

github [dart-lang](https://github.com/dart-lang) / **[sdk](https://github.com/dart-lang/sdk)** / [samples](https://github.com/dart-lang/sdk/tree/master/samples) / [ffi](https://github.com/dart-lang/sdk/tree/master/samples/ffi) / [sqlite](https://github.com/dart-lang/sdk/tree/master/samples/ffi/sqlite) 

### Blog

medium [Integrating C library in a Flutter app using Dart FFI](https://medium.com/flutter-community/integrating-c-library-in-a-flutter-app-using-dart-ffi-38a15e16bc14)

> NOTE: 
>
> 讲的是集成 Dart 和 OpenCV

raywenderlich [Calling Native Libraries in Flutter with Dart FFI](https://www.raywenderlich.com/21512310-calling-native-libraries-in-flutter-with-dart-ffi)

