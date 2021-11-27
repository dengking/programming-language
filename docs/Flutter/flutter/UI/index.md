# docs [Development # User interface](https://docs.flutter.dev/development/ui/widgets-intro)



| flutter     | Android                                                      | iOS                                                          | web                                                          |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
|             | [Flutter for Android developers](https://flutter.dev/docs/get-started/flutter-for/android-devs) | [Flutter for iOS developers](https://flutter.dev/docs/get-started/flutter-for/ios-devs) | [Flutter for web developers](https://flutter.dev/docs/get-started/flutter-for/web-devs) |
| `Widget`    | `View`                                                       | `UIView`                                                     |                                                              |
| declarative | imperative                                                   | imperative                                                   | imperative                                                   |



## Imperative style VS Declarative style

> NOTE: 
>
> 参考: 
>
> flutter [Introduction to declarative UI](https://flutter.dev/docs/get-started/flutter-for/declarative)



## docs [Introduction to widgets](https://docs.flutter.dev/development/ui/widgets-intro)

"Widgets describe what their view should look like given their current configuration and state. When a widget’s state changes, the widget rebuilds its description, which the framework diffs against the previous description in order to determine the minimal changes needed in the underlying render tree to transition from one state to the next."

> NOTE: 
>
> state machine: transition
>
> 当state变化的时候，主要调用"[`build()`](https://api.flutter.dev/flutter/widgets/StatelessWidget/build.html) function"来"rebuilds its description"，`build` 函数相当于是一个factory，它每次都创建一个新的 `Widget` 对象；显然它是由framework自动来进行调用的，即"When a widget’s state changes, the widget rebuilds its description"。
>
> 关于这一点，在  flutter [Introduction to widgets](https://docs.flutter.dev/development/ui/widgets-intro) # [When a widget’s state changes, the widget rebuilds its description](https://docs.flutter.dev/development/ui/widgets-intro) 章节中有如下描述:
>
> ```dart
> void _increment() {
>  setState(() {
>    // This call to setState tells the Flutter framework
>    // that something has changed in this State, which
>    // causes it to rerun the build method below so that
>    // the display can reflect the updated values. If you
>    // change _counter without calling setState(), then
>    // the build method won't be called again, and so
>    // nothing would appear to happen.
>    _counter++;
>  });
> }
> ```
>
> 



### [`build()`](https://api.flutter.dev/flutter/widgets/StatelessWidget/build.html) function

```dart
Widget build(BuildContext context) 
```



> A widget’s main job is to implement a [`build()`](https://api.flutter.dev/flutter/widgets/StatelessWidget/build.html) function, which describes the widget in terms of other, lower-level widgets.

### widget tree

root of widget tree

```dart
void main() {
  runApp(
    const Center(
      child: Text(
        'Hello, world!',
        textDirection: TextDirection.ltr,
      ),
    ),
  );
}
```

1、为了构成widget tree，显然`Widge`类是有`child`属性、入参的。

2、开发一个page的过程，其实就是构建一个tree

### flex widgets 

> NOTE: 
>
> "flex"是"flexible"的缩写

**[`Row`](https://api.flutter.dev/flutter/widgets/Row-class.html), [`Column`](https://api.flutter.dev/flutter/widgets/Column-class.html)**



### `StatefulWidget` and `State` 

You might wonder why `StatefulWidget` and `State` are separate objects. In Flutter, these two types of objects have different life cycles. `Widgets` are temporary objects, used to construct a presentation of the application in its current state. `State` objects, on the other hand, are persistent between calls to `build()`, allowing them to remember information.



In Flutter, change notifications flow “up” the widget hierarchy by way of callbacks, while current state flows “down” to the stateless widgets that do presentation. The common parent that redirects this flow is the `State`. 





### Bringing it all together

> NOTE: 
>
> 一、**Pattern for stateless widgets**
>
> ```dart
> class ShoppingListItem extends StatelessWidget 
> ```
>
> 1、"It stores the values it receives in its constructor in [`final`](https://dart.dev/guides/language/language-tour#final-and-const) member variables, which it then uses during its [`build()`](https://api.flutter.dev/flutter/widgets/StatelessWidget/build.html) function. "
>
> 2、在 `StatelessWidget` 中，存储callback
>
> ```dart
> @override
> Widget build(BuildContext context) {
>  return ListTile(
>    onTap: () {
>      onCartChanged(product, inCart);
>    },
>    leading: CircleAvatar(
>      backgroundColor: _getColor(context),
>      child: Text(product.name[0]),
>    ),
>    title: Text(product.name, style: _getTextStyle(context)),
>  );
> }
> ```
>
> 

## docs [Widget catalog](https://docs.flutter.dev/development/ui/widgets)

> NOTE:
>
> 关于 [Layout widgets](https://docs.flutter.dev/development/ui/widgets/layout)，参见 `Layout` 章节。

## Flutter [Material Components library](https://material.io/develop/flutter) 



