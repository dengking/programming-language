# Enforce sequencing between operand

本文总结能够保证evaluation order的方式。



## stackoverflow [Is comma operator free from side effect?](https://stackoverflow.com/questions/7784774/is-comma-operator-free-from-side-effect)

For example for such statement:

```cpp
c += 2, c -= 1
```

Is it true that c += 2 will be always evaluated first, and c in second expression c-= 1 will always be updated value from expression c += 2?



[A](https://stackoverflow.com/a/7784819)

Yes, it is guaranteed by the standard, as long as that comma is a non-overloaded comma operator. Quoting n3290 §5.18: