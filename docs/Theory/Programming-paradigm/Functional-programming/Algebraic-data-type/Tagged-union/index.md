# Tagged union
简而言之: 就是给type添加一个tag，相比于普通的union，显然它是能够实现dynamic type safety的。

## wikipedia [Tagged union](https://en.wikipedia.org/wiki/Tagged_union)



Only one of the types can be in use at any one time, and a **tag** field explicitly indicates which one is in use. It can be thought of as a type that has several "cases", each of which should be handled correctly when that type is manipulated.

> NOTE: 
>
> dynamic type safety，它的实现方式其实可以联想到C++ 的type erasure，它是以类似的方式实现的dynamic type safety。



Like ordinary [unions](https://en.wikipedia.org/wiki/Union_(computer_science)), tagged unions can save storage by overlapping storage areas for each type, since only one is in use at a time.

> NOTE: union是典型的节省memory的一种措施





A tagged union can be seen as the simplest kind of [self-describing](https://en.wikipedia.org/wiki/Self-describing) [data format](https://en.wikipedia.org/wiki/Comparison_of_data_serialization_formats). The tag of the tagged union can be seen as the simplest kind of [metadata](https://en.wikipedia.org/wiki/Metadata).

> NOTE: 
>
> 这个总结是非常好的
>
> metadata是描述data的data，显然，tagged union的tag是典型的metadata

