# Operations

本文描述对map的各种操作。

## 概述

### fluentcpp [Overview of std::map’s Insertion / Emplacement Methods in C++17](https://www.fluentcpp.com/2018/12/11/overview-of-std-map-insertion-emplacement-methods-in-cpp17/)

Go ahead and try out the C++17 emplacement / insertion methods. They offer a tangible improvement in terms of expressiveness and code safety.

![insert_or_assign try_emplace map C++](https://www.fluentcpp.com/wp-content/uploads/2018/11/insertion_methods1.png)

Here are [examples](https://wandbox.org/permlink/RlhIkZ7ubvgyw16r) of code using them.



### stackoverflow [insert vs emplace vs operator[] in c++ map](https://stackoverflow.com/questions/17172080/insert-vs-emplace-vs-operator-in-c-map)



## TODO Bug

stackoverflow [Calling std::map::emplace() and avoiding unnecessary constructions](https://stackoverflow.com/questions/20830349/calling-stdmapemplace-and-avoiding-unnecessary-constructions)



**Warning:** [In some implementations, option B constructs the pair even if the key is *not* unique, which is probably a bug](http://stackoverflow.com/a/20328676/560648). – [Lightness Races in Orbit](https://stackoverflow.com/users/560648/lightness-races-in-orbit) [Dec 29 '13 at 23:37](https://stackoverflow.com/questions/20830349/calling-stdmapemplace-and-avoiding-unnecessary-constructions#comment31241611_20830492)

stackoverflow [Does map move-insertion guarantee that elements are or are not moved from?](https://stackoverflow.com/questions/20328242/does-map-move-insertion-guarantee-that-elements-are-or-are-not-moved-from)