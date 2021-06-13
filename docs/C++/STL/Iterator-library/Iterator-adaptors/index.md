# 关于本章

本章描述`c++`的`insert_iterator`系列，每种`insert_iterator`都有对应的`inserter`函数来生成`insert_iterator`，显然这是采用的[Object Generator](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Object_Generator) idiom。在STL中，有如下`insert_iterator`：

| `insert_iterator`                                            | `inserter`                                                   |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [std::insert_iterator](https://en.cppreference.com/w/cpp/iterator/insert_iterator) | [std::inserter](https://en.cppreference.com/w/cpp/iterator/inserter) |
| [std::front_insert_iterator](https://en.cppreference.com/w/cpp/iterator/front_insert_iterator) | [std::front_inserter](https://en.cppreference.com/w/cpp/iterator/front_inserter) |
| [std::back_insert_iterator](https://en.cppreference.com/w/cpp/iterator/back_insert_iterator) | [std::back_inserter](https://en.cppreference.com/w/cpp/iterator/back_inserter) |

