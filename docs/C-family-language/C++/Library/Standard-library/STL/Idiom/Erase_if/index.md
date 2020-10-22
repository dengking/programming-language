# 关于本章

本章描述如何从C++ container中删除符合条件的元素，正如维基百科[Erase–remove idiom](https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom)中所描述的：

> A common programming task is to remove all elements that have a certain value or fulfill a certain criterion from a [collection](https://en.wikipedia.org/wiki/Collection_(abstract_data_type)). 

后面为了描述便利，我将这种操作称为`erase_if`；

## Before C++20

C++ STL提供了多种container，需要结合这些container的实现来选择合适的：

### For containers based on an array

- [`std::vector`](https://en.cppreference.com/w/cpp/container/vector) 

- [std::deque](https://en.cppreference.com/w/cpp/container/deque) （它不是完全基于array的）

使用[Erase-remove idiom](./Erase-remove/Erase-remove-idiom.md)。

### For containers based on linked list

- [std::forward_list](https://en.cppreference.com/w/cpp/container/forward_list)
- [std::list](https://en.cppreference.com/w/cpp/container/list)



这些container提供了`remove`、`remove_if`成员函数，使用这些成员函数，可以满足需求；

也可以使用algorithm library提供的`std::remove`、`std::remove_if`，所以它们也可以使用使用[Erase-remove idiom](./Erase-remove/Erase-remove-idiom.md)。

### For containers based on search tree

- [std::set](https://en.cppreference.com/w/cpp/container/set)
- [std::multiset](https://en.cppreference.com/w/cpp/container/multiset)
- [std::map](https://en.cppreference.com/w/cpp/container/map)
- [std::multimap](https://en.cppreference.com/w/cpp/container/multimap)

这些container无法使用使用iterate-delete 方式。

### For containers based on hash table

- [unordered_set](https://en.cppreference.com/w/cpp/container/unordered_set)(C++11) 
- [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map)(C++11) 
- [unordered_multiset](https://en.cppreference.com/w/cpp/container/unordered_multiset)(C++11) 
- [unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)(C++11) 



## C++20



C++20提供了`std::erase_if`系列函数：

- [std::erase, std::erase_if (std::forward_list)](https://en.cppreference.com/w/cpp/container/forward_list/erase2)

  