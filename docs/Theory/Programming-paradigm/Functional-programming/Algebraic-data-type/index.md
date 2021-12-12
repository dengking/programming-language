# Algebraic data type

在 https://clang.llvm.org/docs/LibASTMatchersTutorial.html 中有如下描述:

> Implemented as a DSL powered by macros and templates (see `ASTMatchers.h` if you’re curious), matchers offer the feel of algebraic data types common to functional programming languages.

这吸引了我对algebraic data type的兴趣。

## quora [**What is an algebraic data type?**](https://www.quora.com/What-is-an-algebraic-data-type)



### [A](https://qr.ae/pGq4n1)

> NOTE: 
>
> 一、看了很多，基本上这个回答是最最易懂的；它不仅给出了algebraic data type的概念，还给出了它的implementation: 其他programming language(比如OOP class、union)中如何实现algebraic data type

An algebraic data type is a data type defined out of a combination of two constructions: **products** and **sums**.

**Products**

A product is a way to combine multiple values of different types into one. They’re present in pretty much all languages, often called “structs”, “records” or “tuples”. In object-oriented programming, a class made up of public fields acts as a product type.

Here are a few examples from different languages:

```C++
data Foo = Foo Int String 
type Foo = (Int, String) 
type Foo = { bar : string; baz : int } 
struct Foo { 
  char* bar; 
  int baz; 
} 
```

> NOTE: 
>
> 解释地非常好

**Sums**

A sum type is another way of building larger types out of smaller ones. Instead of combining two values into one, a sum type is a value that is *either* one type or another. It’s the “or” to the product type’s “and”.

Sum types are a bit rarer than products. Many languages with product types have no native version of sum types. In languages that have them, they are called “sum types”, “variants”, “tagged unions” or “enums”.

> NOTE: 解释地非常好

Here are a couple of examples from Haskell and Swift respectively:

```haskell
data Foo = Bar Int 
         | Baz String 
 
enum Foo { 
  case bar(Int) 
  case baz(String) 
} 
```

A number of features many languages have built-in can be expressed with sum types as a library, like booleans:

```haskell
data Bool = False | True 
```

Nullable types:

```haskell
data Nullable a = Null | Just a 
```

Error-handling:

```haskell
data Error err a = Error err | Result a 
```

> NOTE: 
>
> 上述描述让我想到了C++ `std::optional`，显然它是sum type

**Algebraic Data Types**

An algebraic data type, then, is a type built out of products and sums along with any primitive types exposed by the language (like functions and machine-level values like `Int`).



> NOTE: 
>
> 相关问题: quora [**Does C++ have algebraic data types?**](https://www.quora.com/Does-C-have-algebraic-data-types)



## wikipedia [Algebraic data type](https://en.wikipedia.org/wiki/Algebraic_data_type)

### Examples

```haskell
data List a = Nil | Cons a (List a)
```



```haskell
data Tree = Empty
          | Leaf Int
          | Node Tree Tree
```



Operations on algebraic data types can be defined by using [pattern matching](https://en.wikipedia.org/wiki/Pattern_matching) to retrieve the arguments. For example, consider a function to find the depth of a `Tree`, given here in Haskell:

```haskell
 depth :: Tree -> Int
 depth Empty = 0
 depth (Leaf n) = 1
 depth (Node l r) = 1 + max (depth l) (depth r)
```

Algebraic data types are highly suited to implementing [abstract syntax](https://en.wikipedia.org/wiki/Abstract_syntax). For example, the following algebraic data type describes a simple language representing numerical expressions:

> NOTE: 
>
> 非常时候用于描述abstract structure，因为abstract syntax其实也是在描述abstract structure

```haskell
data Expression = Number Int
                | Add Expression Expression
                | Minus Expression Expression
                | Mult Expression Expression
                | Divide Expression Expression
```

### Explanation

> NOTE: 
>
> 其实，这一段主要是在解释pattern match

To do something with a value of this Tree algebraic data type, it is *deconstructed* using a process termed *pattern matching*. It involves *matching* the data with a series of *patterns*. The example function "depth" above pattern-matches its argument with three patterns. When the function is called, it finds the first pattern that matches its argument, performs any variable bindings that are found in the pattern, and evaluates the expression corresponding to the pattern.

> NOTE: 
>
> 对比上述使用Haskell写的相同的程序可以看出，在Haskell中，是不需要写各种if-else的

```C++
 switch on (data.constructor)
   case Empty:
     return 0
   case Leaf:
     let l = data.field1
     return 1
   case Node:
     let l = data.field1
     let r = data.field2
     return 1 + max (depth l) (depth r)
```



## ALGEBRAIC STRUCTURES

这个系列文章，初看还是不错的。

https://jrsinclair.com/articles/2019/algebraic-structures-what-i-wish-someone-had-explained-about-functional-programming/