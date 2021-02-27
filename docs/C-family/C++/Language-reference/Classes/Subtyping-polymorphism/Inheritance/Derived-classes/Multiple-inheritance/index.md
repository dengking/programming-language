# Multiple inheritance



## 两种可能的class hierarchy

C++是支持[multiple inheritance](https://en.wikipedia.org/wiki/Multiple_inheritance)特性的，这就是使得C++的class hierarchy可能非常复杂，下面是两类class hierarchy:

### Diamond and [the diamond problem](https://en.wikipedia.org/wiki/Multiple_inheritance#The_diamond_problem)

```
  A
 / \
B   C
 \ /
  D
```

D inherit from B

D inherit from C

B inherit from A

C inherit from A

这种class hierarchy是存在[The diamond problem](https://en.wikipedia.org/wiki/Multiple_inheritance#The_diamond_problem)的。

### Tree

```
A1  A2
|   |
B   C
 \ /
  D
```

D inherit from B

D inherit from C

B inherit from A1

C inherit from A2

# 

## When to use multiple inheritance

1、mixin multiple、multiple CRTP

参见 `Variadic-CRTP`、

2、implement multiple interface



## TODO

### stackoverflow [How to use parameterized base class constructor downstream of virtual inheritance](https://stackoverflow.com/questions/34732563/how-to-use-parameterized-base-class-constructor-downstream-of-virtual-inheritanc)