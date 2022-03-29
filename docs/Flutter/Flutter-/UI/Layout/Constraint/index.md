# Constraint

## docs [Understanding constraints](https://docs.flutter.dev/development/ui/layout/constraints)

**Constraints go down. Sizes go up. Parent sets position.**

> NOTE: 
>
> "go down":  从 parent 到 children
>
> "go up": 从 children 到 parent

In more detail:

1、A widget gets its own **constraints** from its **parent**. A *constraint* is just a set of 4 doubles: a minimum and maximum width, and a minimum and maximum height.

2、Then the widget goes through its own list of **children**. One by one, the widget tells its children what their **constraints** are (which can be different for each child), and then asks each child what size it wants to be.

3、Then, the widget positions its **children** (horizontally in the `x` axis, and vertically in the `y` axis), one by one.

4、And, finally, the widget tells its parent about its own **size** (within the original constraints, of course).



## flutter logical pixel

stackoverflow [How does Flutter calculate pixels for different resolutions?](https://stackoverflow.com/questions/44572330/how-does-flutter-calculate-pixels-for-different-resolutions)

api [size property](https://api.flutter.dev/flutter/widgets/MediaQueryData/size.html)

百度: flutter logical pixel