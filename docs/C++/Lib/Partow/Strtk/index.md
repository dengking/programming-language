# StrTk



## [Doc](http://www.partow.net/programming/strtk/index.html)

### Introduction To StrTk

StrTk provides two common tokenization concepts, a split function and a token iterator. Both these concepts require the user to provide a delimiter predicate and an iterator range over which the process will be carried out.

> NOTE: "an iterator range over which the process will be carried out"是指传入iterator range来表示在哪个range内执行tokenization，这是c++ generic programming的常用做法，后续我们将此称为iterator-based generic programming，strtk是基于此而开发的

The tokenization process can be further parametrized by switching between "**compressed delimiters**" or "**no compressed delimiters**" mode.



### Delimiters

Two forms of delimiters are supported and they are **single delimiter predicate** and **multiple delimiters predicate** otherwise known as **SDP** and **MDP** respectively. Essentially an SDP is where there is only one **type** that can break or fragment the sequence, where as with MDPs there is more than one unique type that can break the sequence. 

> NOTE: 这里引入了一个新的概念：SDP、MDP，它们是基于“type”的。

It is possible to represent a SDP using the MDP, however from a performance POV having separate predicates is far more efficient. Additionally for strings based on char or unsigned char (8-bit versions) there is a MDP that has a look-up complexity of O(1) making it greatly more efficient than the basic MDP.

> NOTE: 最后一段话的意思是：对于“strings based on char or unsigned char ”，strtk提供了一个优化的MDP，这个MDP使用look-up complexity是O(1)的算法，它比普通的MDP更优。在下面的Multiple Char Delimiter Predicate会对此进行说明。

#### Single Delimiter Predicate



#### Multiple Delimiter Predicate

Instantiation requires specialization of type and construction requires a sequence of potential delimiters through a **range described by iterators**.

> NOTE: strtk中，很多function都是基于range的，**range described by iterators**为“iterator-based generic programming”

```c++
strtk::multiple_delimiter_predicate<typename T>(Iterator begin, Iterator end);

std::string str_delimiters = " ,.;:<>'[]{}()_?/'`~!@#$%^&*|-_\"=+";
strtk::multiple_delimiter_predicate mdp1(str_delimiters.begin(),str_delimiters.end());

unsigned int uint_delimiters[5] = {1, 10, 101, 1010, 10101};
strtk::multiple_delimiter_predicate<unsigned int> mdp2(uint_delimiters,uint_delimiters + 5);
```

#### Multiple Char Delimiter Predicate

Instantiation requires an iterator range based on either unsigned char or char. This delimiter is more efficient than the simple MDP as it has a predicate evaluation of **O(1)** due to the use of a lookup-table as opposed to **O(n)** where n is the number of delimiters. Performance increase is seen as more delimiters are used.

> NOTE: 显然，对于Mutiple char delimiter predicate，通过使用lookup-up table来使得predicate evaluation 的complexity变为**O(1)**。其实看到了这里，我有一个疑问：对于其他的Multiple Delimiter Predicate，为什么不全部都使用look-up table呢？

```c++
strtk::multiple_char_delimiter_predicate(const std::string&)

strtk::multiple_char_delimiter_predicate(const std::string::const_iterator begin,
                                         const std::string::const_iterator end)

//usage:
strtk::multiple_char_delimiter_predicate predicate(" .;?");
```



### Split

This is a function that performs tokenization over an entire sequence in one go. **strtk::split** takes a sequence through a range of iterators or in the case of a string through a reference to its instance, a delimiter predicate and an output iterator or otherwise known as a type **sink**. It populates the **output iterator** with the tokens it extracts. The tokens in this case are `std::pairs` of iterators for the sequence.

> NOTE: `split`函数的入参有三个：
>
> - a sequence through a range of iterators 
> - a delimiter predicate 
> - an output iterator 
>
> token是用`std::pairs` of iterators 来表达的
>
> 可以看到，上述三个parameter、token都是基于iterator的。

## Code

[strtk](https://github.com/ArashPartow/strtk)