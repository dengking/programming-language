## [Flutter](https://api.flutter.dev/flutter/index.html) / [widgets library](https://api.flutter.dev/flutter/widgets/widgets-library.html)

### [GlobalKey](https://api.flutter.dev/flutter/widgets/GlobalKey-class.html)



#### stackoverflow [How to use `GlobalKey` to maintain widgets' states when changing parents?](https://stackoverflow.com/questions/56895273/how-to-use-globalkey-to-maintain-widgets-states-when-changing-parents)



#### usage

##### constructor的入参: key

flutter的widget的第一个入参一般都是key，从 https://github.com/iampawan/FlutterForms/blob/master/lib/main.dart 中的code可以看出:

```dart
class _FormPageState extends State<FormPage> {
  final scaffoldKey = new GlobalKey<ScaffoldState>();
  final formKey = new GlobalKey<FormState>();
    @override
  Widget build(BuildContext context) {
    return new Scaffold(
        key: scaffoldKey,
        appBar: new AppBar(
          title: new Text("Form Page"),
        ),
        body: new Padding(
          padding: const EdgeInsets.all(20.0),
          child: new Form(
            key: formKey,
  }
}
```

### [Container class](https://api.flutter.dev/flutter/widgets/Container-class.html)

> NOTE: 
>
> 一、在 docs [Layouts in Flutter](https://docs.flutter.dev/development/ui/layout) 中，对 [`Container`](https://api.flutter.dev/flutter/widgets/Container-class.html) 有着非常好的介绍
>
> 二、在 docs [Introduction to widgets](https://docs.flutter.dev/development/ui/widgets-intro) 中，对 [`Container`](https://api.flutter.dev/flutter/widgets/Container-class.html) 有着非常好的介绍

## [Flutter](https://api.flutter.dev/flutter/index.html) / [material library](https://api.flutter.dev/flutter/material/material-library.html)

### [Scaffold](https://api.flutter.dev/flutter/material/Scaffold-class.html)



### [TabController class](https://api.flutter.dev/flutter/material/TabController-class.html)

## [Flutter](https://api.flutter.dev/flutter/index.html) / [painting library](https://api.flutter.dev/flutter/painting/painting-library.html)

### [EdgeInsets class](https://api.flutter.dev/flutter/painting/EdgeInsets-class.html) 

> NOTE: 
>
> "edge"的意思是"边"
>
> "inset"的意思是"内凹、嵌入物、（套印在大图片、地图等中的）小图"