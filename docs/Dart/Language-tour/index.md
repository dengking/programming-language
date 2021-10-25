# [A tour of the Dart language](https://dart.dev/guides/language/language-tour)



## Built-in types

### Lists

#### spread operator (`...`) 

Dart 2.3 introduced the **spread operator** (`...`) and the **null-aware spread operator** (`...?`), which provide a concise way to insert multiple values into a collection.

For example, you can use the spread operator (`...`) to insert all the values of a list into another list:

```dart
var list = [1, 2, 3];
var list2 = [0, ...list];
assert(list2.length == 4);
```

> NOTE: 
>
> 它其实就相当于Python的 `*`

## Operators

### Cascade notation

> NOTE: 在学习如下代码时，发现的这种用法:
>
> ```C++
>  final interactiveCppRequests = ReceivePort()..listen(DartRecvPortHandler);
> ```
>
> 



##  Asynchrony support

> NOTE: 
>
> 在 `Asynchronous-programming` 章节中进行了统一的整理。
