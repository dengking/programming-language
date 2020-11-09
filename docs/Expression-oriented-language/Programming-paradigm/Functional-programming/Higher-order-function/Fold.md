# Fold 



## infogalactic [Fold (higher-order function)](https://infogalactic.com/info/Fold_(higher-order_function))

In [functional programming](https://infogalactic.com/info/Functional_programming), **fold** – also known variously as **reduce**, **accumulate**, **aggregate**, **compress**, or **inject** – refers to a family of [higher-order functions](https://infogalactic.com/info/Higher-order_function) that [analyze](https://infogalactic.com/info/Analysis) a [recursive](https://infogalactic.com/info/Recursive_data_type) data structure and through use of a given combining operation, recombine the results of [recursively](https://infogalactic.com/info/Recursion) processing its constituent parts, building up a return value. 

Typically, a fold is presented with a combining [function](https://infogalactic.com/info/Subroutine), a top [node](https://infogalactic.com/info/Node_(computer_science)) of a [data structure](https://infogalactic.com/info/Data_structure), and possibly some default values to be used under certain conditions. The fold then proceeds to combine elements of the data structure's [hierarchy](https://infogalactic.com/info/Hierarchy), using the function in a systematic way.

### Folds in various languages

|                           Language                           |                  Left fold                   |                          Right fold                          | Left fold without initial value |             Right fold without initial value              |                            Notes                             |
| :----------------------------------------------------------: | :------------------------------------------: | :----------------------------------------------------------: | :-----------------------------: | :-------------------------------------------------------: | :----------------------------------------------------------: |
|         [C++](https://infogalactic.com/info/C%2B%2B)         | `std::accumulate(begin, end, initval, func)` |        `std::accumulate(rbegin, rend, initval, func)`        |                                 |                                                           | in header `<numeric>` *begin*, *end*, *rbegin*, *rend* are iterators *func* can be a [function pointer](https://infogalactic.com/info/Function_pointer) or a [function object](https://infogalactic.com/info/Function_object) |
| [Python](https://infogalactic.com/info/Python_(programming_language)) 3.x |   `functools.reduce(func, list, initval)`    | `functools.reduce(lambda x,y: func(y,x), reversed(list), initval)` | `functools.reduce(func, list)`  | `functools.reduce(lambda x,y: func(y,x), reversed(list))` | In module *functools*.[[2\]](https://infogalactic.com/info/Fold_(higher-order_function)#cite_note-2) |