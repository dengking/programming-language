# Assemble polymorphism and multiple dispatch

1、C++语言原生并不不支持multiple dispatch，有的时候，需要进行multiple dispatch，因此一个思路就是: assemble C++中的各种 polymorphism、甚至custom polymorphism、Emulating(模拟) multiple dispatch using multiple single dispatch

参见 `Programming-paradigm\Common\Abstraction-and-polymorphism\Polymorphism\Implementation` 章节

2、借助external polymorphism

参见 `wiki.c2-External-Polymorphism` 章节

3、visitor-pattern，参见 `C++\Pattern\Visitor-pattern` 章节