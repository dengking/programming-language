# Building layout



## docs [Layouts in Flutter](https://docs.flutter.dev/development/ui/layout)

### What's the point?

Widgets are used for both layout and UI elements.

> NOTE: 
>
> 这段话是非常重要的
>
> 是否有专门进行layout、专门进行UI element的Widgets ？有的，在原文有这样的描述:
>
> > Choose from a variety of [layout widgets](https://docs.flutter.dev/development/ui/widgets/layout) based on how you want to align or constrain the visible widget, as these characteristics are typically passed on to the contained widget.
>
> layout widget是用于"align or constrain the visible widget"
>
> 在下面的

### Lay out a widget

#### 1. Select a layout widget

#### 2. Create a visible widget

#### 3. Add the visible widget to the layout widget

> NOTE: 这个是非常重要的，这是flutter的使用模式

#### 4. Add the layout widget to the page



### Lay out multiple widgets vertically and horizontally

> NOTE: 
>
> 很多都是基于此来进行布局的

#### Aligning widgets

> NOTE: 
>
> 对准、对齐

#### Sizing widgets



#### Packing widgets

> NOTE: 
>
> 聚集在一起

#### Nesting rows and columns

> NOTE:
>
> ```dart
> padding: const EdgeInsets.all(20), 
> padding: const EdgeInsets.fromLTRB(20, 30, 20, 20),
> ```
>
> 



## docs [Layout widgets](https://docs.flutter.dev/development/ui/widgets/layout)

> NOTE: 
>
> 一、典型的全部都是嵌套，将所有的widget进行嵌套、将布局进行嵌套、将样式进行嵌套
>
> 二、layout constraint

### Single-child layout widgets

Positioning: 

1、[Align](https://api.flutter.dev/flutter/widgets/Align-class.html)

> NOTE: 其中给出的例子就是在 [Container](https://api.flutter.dev/flutter/widgets/Container-class.html) 中使用 [Align](https://api.flutter.dev/flutter/widgets/Align-class.html)

2、[Center](https://api.flutter.dev/flutter/widgets/Center-class.html)

3、[Padding](https://api.flutter.dev/flutter/widgets/Padding-class.html)

When passing layout constraints to its child, padding shrinks the constraints by the given padding, causing the child to layout at a smaller size. Padding then sizes itself to its child's size, inflated by the padding, effectively creating empty space around the child.

> NOTE: 
>
> "当将布局约束传递给它的孩子时，padding 会通过给定的 padding 缩小约束，导致孩子以更小的尺寸进行布局。 Padding 然后根据其孩子的尺寸调整自己的大小，由 padding 膨胀，有效地在孩子周围创造空白空间。"
>
> "padding"的表面意思是"填充"



Why use a [Padding](https://api.flutter.dev/flutter/widgets/Padding-class.html) widget rather than a [Container](https://api.flutter.dev/flutter/widgets/Container-class.html) with a [Container.padding](https://api.flutter.dev/flutter/widgets/Container/padding.html) property?

> NOTE: 
>
>  [Padding](https://api.flutter.dev/flutter/widgets/Padding-class.html) 是比 [Container](https://api.flutter.dev/flutter/widgets/Container-class.html) 跟低粒度的widget。

### Multi-child layout widgets

1、[Column](https://api.flutter.dev/flutter/widgets/Column-class.html)、[Row](https://api.flutter.dev/flutter/widgets/Row-class.html)

> NOTE: 
>
> 与它相关的一个widget是 [Expanded](https://api.flutter.dev/flutter/widgets/Expanded-class.html)

2、[Wrap](https://api.flutter.dev/flutter/widgets/Wrap-class.html)