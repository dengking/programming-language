# Iterator library



## geeksforgeeks[Introduction to Iterators in C++](https://www.geeksforgeeks.org/introduction-iterators-c/)

![](./C_Iterator.jpg)

Now each one of these iterators are not supported by all the containers in STL, different containers support different iterators, like vectors support [Random-access iterators](https://www.geeksforgeeks.org/random-access-iterators-in-cpp/), while lists support [bidirectional iterators.](https://www.geeksforgeeks.org/bidirectional-iterators-in-cpp/) The whole list is as given below:

![](./C_Iterator_Support.jpg)

**Types of iterators**: Based upon the functionality of the iterators, they can be classified into five major categories:

1. [**Input Iterators**](https://www.geeksforgeeks.org/input-iterators-in-cpp/): They are the weakest of all the iterators and have very limited functionality. They can only be used in a single-pass algorithms, i.e., those algorithms which process the container sequentially, such that no element is accessed more than once.
2. [**Output Iterators**](https://www.geeksforgeeks.org/output-iterators-cpp/): Just like [input iterators](https://www.geeksforgeeks.org/input-iterators-in-cpp/), they are also very limited in their functionality and can only be used in single-pass algorithm, but not for accessing elements, but for being assigned elements.
3. [**Forward Iterator**](https://www.geeksforgeeks.org/forward-iterators-in-cpp/): They are higher in the hierarachy than[ input](https://www.geeksforgeeks.org/input-iterators-in-cpp/) and [output iterators](https://www.geeksforgeeks.org/output-iterators-cpp/), and contain all the features present in these two iterators. But, as the name suggests, they also can only move in a forward direction and that too one step at a time.
4. [**Bidirectional Iterators**](https://www.geeksforgeeks.org/bidirectional-iterators-in-cpp/): They have all the features of[ forward iterators](https://www.geeksforgeeks.org/forward-iterators-in-cpp/) along with the fact that they overcome the drawback of [forward iterators](https://www.geeksforgeeks.org/forward-iterators-in-cpp/), as they can move in both the directions, that is why their name is bidirectional.
5. [**Random-Access Iterators**](https://www.geeksforgeeks.org/random-access-iterators-in-cpp/): They are the most powerful iterators. They are not limited to moving sequentially, as their name suggests, they can randomly access any element inside the container. They are the ones whose functionality are same as pointers.

![](./iteratorOperation.jpg)

## cppreference [Iterator library](https://en.cppreference.com/w/cpp/iterator)





## cppreference [Named requirements of  iterator](https://en.cppreference.com/w/cpp/named_req#Iterator)