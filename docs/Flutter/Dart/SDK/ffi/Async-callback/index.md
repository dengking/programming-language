# Cannot invoke native callback outside an isolate

## github [problem when using a FFI 'C' callback function #40529](https://github.com/dart-lang/sdk/issues/40529)

### **[dcharkes](https://github.com/dcharkes)** commented [on 11 Feb 2020](https://github.com/dart-lang/sdk/issues/40529#issuecomment-584530622)

Yes, doing a callback from another thread is not supported. Dart has one mutator thread per isolate. If a Dart thread is already running in the isolate, and you would call back from C into Dart with another thread, there would be 2 threads executing Dart code in a single isolate. That would break the concurrency assumptions.

Instead, you have to call back into Dart on the main Dart thread. One can notify the main Dart thread asynchronously through a SendPort. Please check this sample: https://dart-review.googlesource.com/c/sdk/+/134704.

## github [[vm/ffi] Support asynchronous callbacks #37022](https://github.com/dart-lang/sdk/issues/37022)

### **[sjindel-google](https://github.com/sjindel-google)** commented [on 21 May 2019](https://github.com/dart-lang/sdk/issues/37022#issue-446301175) • edited by dcharkes 



 **Update March 12, 2020**: Asynchronous callbacks can be done through the native ports mechanism. See the samples in [samples/ffi/async](https://github.com/dart-lang/sdk/tree/master/samples/ffi/async) and corresponding [c code](https://github.com/dart-lang/sdk/blob/master/runtime/bin/ffi_test/ffi_test_functions_vmspecific.cc#L370-L828). In the future we would like to add a more concise syntax that requires less boilerplate code.

An asynchronous callback can be invoked outside the parent isolate and schedules a microtask there.



## juejin [字节小哥带你揭秘Dart VM魔术盒](https://juejin.cn/post/6960932663095984136)



### Dart VM Isolate

在Dart VM中任何dart代码都是运行在某个isolate中的，每个isolate都是独立的，它们都有自己的存储空间、主线程和各个辅助线程，isolate之间互不影响。在Dart VM中可能同时运行着多个isolate，但它们不能直接共享数据，可以通过**端口**（ports：Dart VM中的一个概念，和网络中的端口不一样）相互交流信息。

![img](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/ce5e625549e7486f89629b332aeaad44~tplv-k3u1fbpfcp-watermark.awebp)

从上面这张图片中不难看出在一个isolate中主要包括以下几个部分：

- Heap：存储dart代码运行过程中创建的所有object，并由GC（垃圾回收）线程管理。
- Mutator Thread：主线程，负责执行dart代码。
- Helper Thread：辅助线程，完成Dart VM中对isolate的一些管理、优化等任务。

在这里我们再深入探讨一下isolate和OS thread的关系，实际上这是十分复杂和不确定的，因为这取决于平台特性和VM被打包进应用的方式，但是有以下三点是确定的：

- 一个OS thread一次只能进入一个isolate，若想进入其他的isolate必须先从当前isolate退出。
- 一个isolate一次只能关联一个mutator thread，mutator thread用于执行dart代码和调用VM中public的C API。
- 一个isolate可以同时关联多个helper thread，比如JIT编译线程、GC线程等。

实际上Dart VM在内部维护了一个全局的线程池`ThreadPool`来管理OS thread，所有创建线程的请求都被描述成发向线程池的`ThreadPool::Task`，比如GC回收内存时发送请求`SweeperTask`，线程池会首先检查池中是否有可用的OS thread，有的话则直接复用，没有的话则会创建一个新的线程。




## github [sdk](https://github.com/dart-lang/sdk)/[samples](https://github.com/dart-lang/sdk/tree/master/samples)/[ffi](https://github.com/dart-lang/sdk/tree/master/samples/ffi)/[async](https://github.com/dart-lang/sdk/tree/master/samples/ffi/async)/



### [**sample_async_callback.dart**](https://github.com/dart-lang/sdk/blob/master/samples/ffi/async/sample_async_callback.dart)

**生产者** 和 **消费者**

需要首先建立起 **生产者** 和 **消费者** 之间的联系



#### receiver

通过 `ReceivePort` 来获得 `sendPort`

```dart
final interactiveCppRequests = ReceivePort()..listen(requestExecuteCallback);
```

`requestExecuteCallback` 是收到消息的时候会被执行的**回调函数**

```dart
void requestExecuteCallback(dynamic message) {
  final int work_address = message;
  final work = Pointer<Work>.fromAddress(work_address);
  print("Dart:   Calling into C to execute callback ($work).");
  executeCallback(work);
  print("Dart:   Done with callback.");
}
```



```dart
final executeCallback = dl.lookupFunction<Void Function(Pointer<Work>),
    void Function(Pointer<Work>)>('ExecuteCallback');
```



#### sender

```dart
  registerCallback1(nativePort, callback1FP);
  registerCallback2(nativePort, callback2FP);
```

`callback1FP`、`callback2FP` 是指向dart函数的function pointer



```dart
final registerCallback1 = dl.lookupFunction<
        Void Function(Int64 sendPort,
            Pointer<NativeFunction<IntPtr Function(IntPtr)>> functionPointer),
        void Function(int sendPort,
            Pointer<NativeFunction<IntPtr Function(IntPtr)>> functionPointer)>(
    'RegisterMyCallbackBlocking');

final registerCallback2 = dl.lookupFunction<
        Void Function(Int64 sendPort,
            Pointer<NativeFunction<Void Function(IntPtr)>> functionPointer),
        void Function(int sendPort,
            Pointer<NativeFunction<Void Function(IntPtr)>> functionPointer)>(
    'RegisterMyCallbackNonBlocking');
```



### [**sample_native_port_call.dart**](https://github.com/dart-lang/sdk/blob/master/samples/ffi/async/sample_native_port_call.dart)

这个例子比较简单，它对应的C代码在 `ffi_test_functions_vmspecific.cc` 中，在其中有如下描述:

```dart
// Functions for async callbacks example.
//
// sample_native_port_call.dart
```



```C++
class SimulateWork2 
```

它仅仅是管理两个thread:

1、`thread1` 执行 `Work1_2`

2、`thread2` 执行 `Work2_2`

它的`Start`就是启动thread，它的`Stop`就是停止thread。





## dart isolate

### api.dart [dart:isolate library](https://api.dart.dev/stable/2.14.0/dart-isolate/dart-isolate-library.html) 

Concurrent programming using *isolates*: independent workers that are similar to threads but don't share memory, communicating only via messages.

