# [The Boost.Iterator Library](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/index.html) 

## [New-Style Iterators](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/index.html#id23)

The iterator categories defined in C++98 are extremely limiting because they bind together two orthogonal（正交） concepts: traversal and element access. For example, because a random access iterator is required to return a reference (and not a proxy) when dereferenced, it is impossible to capture the capabilities of `vector<bool>::iterator` using the C++98 categories. This is the infamous "`vector<bool>` is not a container, and its iterators aren't random access iterators", debacle about which Herb Sutter wrote two papers for the standards comittee ([n1185](http://www.gotw.ca/publications/N1185.pdf) and [n1211](http://www.gotw.ca/publications/N1211.pdf)), and a [Guru of the Week](http://www.gotw.ca/gotw/050.htm). 

> NOTE: 上面这段话的意思是：traversal 和 element access是正交的，两者应该是独立的，应该分开来，但是C++98的iterator却将两者杂糅在一起了，这段话中以random access iterator为例对此进行了说明，random access iterator显然是具备traversal 的概念的，要求它return a reference (and not a proxy) when dereferenced则是要求它具备element access的概念。

New-style iterators go well beyond patching up `vector<bool>`, though: there are lots of other iterators already in use which can't be adequately represented by the existing concepts. For details about the new iterator concepts, see our

> [Standard Proposal For New-Style Iterators](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.pdf))



## [Iterator Facade and Adaptor](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/index.html#id24)



## [Specialized Adaptors](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/index.html#id25)