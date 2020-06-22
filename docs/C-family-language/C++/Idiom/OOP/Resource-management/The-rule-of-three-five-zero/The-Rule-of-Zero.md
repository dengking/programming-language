# [The Rule of Zero](http://blog.florianwolters.de/educational/2015/01/31/The_Rule_of_Zero/)

## Introduction

Modern C++ code should use the *Resource Acquisition Is Initialisation (RAII)* / *Resource Release Is Destruction (RRID)* idiom. Bjarne Stroustrop, the inventor of the C++ language describes RAII as follows:

> The basic idea [of RAII] is to represent a resource by a local object, so that the local object’s destructor will release the resource. That way, the programmer cannot forget to release the resource.

*Source:* [Bjarne Stroustrup’s C++ Style and Technique FAQ](http://www.stroustrup.com/bs_faq2.html#finally)

RAII is also described in detail as *Item 13: Use objects to manage resources* in Scott Meyers book “Effective C++”. Mr. Meyers suggests the following:

> To prevent resource leaks, use RAII objects that acquire resources in their constructors and release them in their destructors.

*Source:* Meyers, Scott. Effective C++, 3rd Edition, p. 66.

## Resource Management Types

In modern C++, the following four types of objects from a resource management perspective can be categorized:

- Copyable, but not moveable.
- Both copyable and moveable.
- Moveable, but not copyable.
- Neither copyable nor moveable.

> NOTE: copyable和moveable的组合

The following matrix illustrates the four cases in a more compact form.

|            | Copyable | Uncopyable |
| :--------- | :------- | :--------- |
| Moveable   | {C, M}   | {UC, M}    |
| Unmoveable | {C, UM}  | {UC, UM}   |

To define the behavior of a class regarding **copy** and **move** semantics, modern C++ offers the following constructs:

- [Copy constructor](http://en.cppreference.com/w/cpp/language/copy_constructor)
- [Copy assignment operator](http://en.cppreference.com/w/cpp/language/as_operator)
- [Move constructor](http://en.cppreference.com/w/cpp/language/move_constructor)
- [Move assignment operator](http://en.cppreference.com/w/cpp/language/move_operator)

The following rules can be applied for the resource management types:

### Uncopyable (**UC**)

- Declare the **copy constructor** as deleted.

*Example:*

```c++
MyObject(MyObject const& kSource) = delete;
```

- Declare the **assignment operator** as deleted.

*Example:*

```c++
MyObject& operator=(MyObject const& kRhs) = delete;
```

### Unmoveable (**UM**)

- Declare the move constructor as deleted.

*Example:*

```c++
MyObject(MyObject&& source) = delete;
```

- Declare the move operator as deleted.

```c++
MyObject& operator=(MyObject&& rhs) = delete;
```

### Copyable (**C**)

- If using automatic resource management: Apply *The Rule of Zero*.
- If using manual resource management: Apply *The Rule of Three*.

### Moveable (**M**)

- If using automatic resource management: Apply *The Rule of Zero*.
- If using manual resource management: Apply *The Rule of Five*.



Instead of using `delete`, the mentioned functions can be declared with`private ` visibility. This allows to implement uncopyable objects if using an older standard, such as C++98. 

## The Rule of […]

A short description with source code examples of the *The Rule of Three*, *The Rule of Five* and *The Rule of Zero* can be found at [cppreference.com](http://en.cppreference.com/w/cpp/language/rule_of_three). [This Stack Overflow](https://stackoverflow.com/questions/4782757/rule-of-three-becomes-rule-of-five-with-c11) question also deals with the three rules.

I will summarize each rule in this article. Please refer to the linked information resources for more in-depth information.

### The Rule of Three

```c++
/**
 * Demonstrates *The Rule of Three* C++ idiom.
 *
 * @file the_rule_of_three.h
 * @author Florian Wolters <wolters.fl@gmail.com>
 *
 * @section License
 *
 * Copyright Florian Wolters 2015 (http://blog.florianwolters.de).
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef FW_IDIOM_THE_RULE_OF_THREE_H_
#define FW_IDIOM_THE_RULE_OF_THREE_H_

#include <cstring>
#include <utility>

namespace fw {
namespace idiom {

/**
 * The TheRuleOfThree class demonstrates *The Rule of Three* idiom.
 *
 * @author Florian Wolters <wolters.fl@gmail.com>
 */
class TheRuleOfThree final {
 public:
  /**
   * Initializes a new instance of the TheRuleOfThree class with the specified
   * C-style string.
   *
   * @param kValue The C-style string.
   */
  explicit TheRuleOfThree(char const* kValue)
      : resource_{new char[std::strlen(kValue) + 1]} {
    std::strcpy(resource_, kValue);
  }

  /**
   * Finalizes an instance of the TheRuleOfThree class.
   *
   * This is the destructor.
   */
  ~TheRuleOfThree() {
    delete[] resource_;
  }

  /**
   * Initializes a new instance of the TheRuleOfThree class from the specified
   * TheRuleOfThree.
   *
   * This is the copy constructor.
   *
   * @param kValue The TheRuleOfThree to copy.
   */
  TheRuleOfThree(TheRuleOfThree const& kOther)
      : resource_{new char[std::strlen(kOther.resource_) + 1]} {
    std::strcpy(resource_, kOther.resource_);
  }

  /**
   * Assigns the specified TheRuleOfThree to this TheRuleOfThree.
   *
   * This is the copy assignment operator.
   *
   * @param kValue The TheRuleOfThree to assign to this
   *    TheRuleOfThree.
   */
  TheRuleOfThree& operator=(TheRuleOfThree& kOther) {
    std::swap(resource_, kOther.resource_);

    return *this;
  }

 private:
  /**
   * The resource (a raw pointer to a character) to handle by this class.
   */
  char* resource_;
};

}  // namespace idiom
}  // namespace fw

#endif  // FW_IDIOM_THE_RULE_OF_THREE_H_

int main() {
  using fw::idiom::TheRuleOfThree;

  // Complete constructor.
  TheRuleOfThree the_rule_of_three{"hello, world"};

  // Copy constructor.
  TheRuleOfThree copy{the_rule_of_three};

  // Copy assignment operator.
  copy = the_rule_of_three;

  // Destructor(s).
}
```

[the_rule_of_three.h](https://gist.github.com/FlorianWolters/26f0ea1117f34866f2a2#file-the_rule_of_three-h) hosted with ❤ by [GitHub](https://github.com/)



### The Rule of Five

I recommend reading the article [The Rule of The Big Four (and a half) – Move Semantics and Resource Management](https://blog.feabhas.com/2015/01/the-rule-of-the-big-four-and-a-half-move-semantics-and-resource-management) by Glennan Carnie.

*The Rule of Five* allows to implement **{C, M}**, **{UC, M}**, **{C, UM}** and **{UC, UM}**.

> If a class has a user-defined destructor, user-defined copy constructor or user defined copy assignment operator, the move constructor and the move assignment operator have to be also be implemented to realize move semantics.

**Example:**

```c++
/**
 * Demonstrates *The Rule of Five* C++ idiom.
 *
 * @file the_rule_of_five.h
 * @author Florian Wolters <wolters.fl@gmail.com>
 *
 * @section License
 *
 * Copyright Florian Wolters 2015 (http://blog.florianwolters.de).
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef FW_IDIOM_THE_RULE_OF_FIVE_H_
#define FW_IDIOM_THE_RULE_OF_FIVE_H_

#include <cstring>
#include <utility>

namespace fw {
namespace idiom {

/**
 * The TheRuleOfFive class demonstrates *The Rule of Five* idiom.
 *
 * @author Florian Wolters <wolters.fl@gmail.com>
 */
class TheRuleOfFive final {
 public:
  /**
   * Initializes a new instance of the TheRuleOfFive class with the specified
   * C-style string.
   *
   * @param kValue The C-style string.
   */
  explicit TheRuleOfFive(char const* kValue)
      : resource_{new char[std::strlen(kValue) + 1]} {
    std::strcpy(resource_, kValue);
  }

  /**
   * Finalizes an instance of the TheRuleOfFive class.
   *
   * This is the destructor.
   */
  ~TheRuleOfFive() {
    delete[] resource_;
  }

  /**
   * Initializes a new instance of the TheRuleOfFive class from the specified
   * TheRuleOfFive.
   *
   * This is the copy constructor.
   *
   * @param kValue The TheRuleOfFive to copy.
   */
  TheRuleOfFive(TheRuleOfFive const& kOther)
      : resource_{new char[std::strlen(kOther.resource_) + 1]} {
    std::strcpy(resource_, kOther.resource_);
  }

  /**
   * Assigns the specified TheRuleOfFive to this TheRuleOfFive.
   *
   * This is the copy assignment operator.
   *
   * @param kValue The TheRuleOfFive to assign to this TheRuleOfFive.
   */
  TheRuleOfFive& operator=(TheRuleOfFive& kOther) {
    std::swap(resource_, kOther.resource_);

    return *this;
  }

  /**
   * Initializes a new instance of the TheRuleOfFive class from the specified
   * TheRuleOfFive.
   *
   * This is the move constructor.
   *
   * @param kValue The TheRuleOfFive to copy.
   */
  TheRuleOfFive(TheRuleOfFive&& other) : resource_{other.resource_}  {
    other.resource_ = nullptr;
  }

  /**
   * Assigns the specified TheRuleOfFive to this TheRuleOfFive.
   *
   * This is the move assignment operator.
   *
   * @param kValue The TheRuleOfFive to assign to this TheRuleOfFive.
   */
  TheRuleOfFive& operator=(TheRuleOfFive&& other) {
    std::swap(resource_, other.resource_);

    return *this;
  }

 private:
  /**
   * The resource (a raw pointer to a character) to handle by this class.
   */
  char* resource_;
};

}  // namespace idiom
}  // namespace fw

#endif  // FW_IDIOM_THE_RULE_OF_FIVE_H_

/**
 * Runs the application.
 *
 * @return Always `0`.
 */
int main() {
  using fw::idiom::TheRuleOfFive;

  // Complete constructor.
  TheRuleOfFive the_rule_of_five_defaults{"hello, world"};

  // Copy constructor.
  TheRuleOfFive copy{the_rule_of_five_defaults};

  // Move constructor.
  TheRuleOfFive move{std::move(copy)};

  // Copy assignment operator.
  copy = the_rule_of_five_defaults;

  // Move assignment operator (from rvalue).
  move = TheRuleOfFive{"foo"};

  // Move assignment operator (from lvalue).
  move = std::move(copy);

  // Destructor(s).
}
```

[the_rule_of_five.h](https://gist.github.com/FlorianWolters/26f0ea1117f34866f2a2#file-the_rule_of_five-h) hosted with ❤ by [GitHub](https://github.com/)



### The Rule of Zero

*The Rule of Zero* was introduced by R. Martinho Fernandes on 15 August 2012. It is defined as follows:

> Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership. Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators.

Scott Meyers corrects the definition in his blog post [A Concern about the Rule of Zero](http://scottmeyers.blogspot.fr/2014/03/a-concern-about-rule-of-zero.html) as follows:

> Classes that **declare** custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership. Other classes should not **declare** custom destructors, copy/move constructors or copy/move assignment operators.

This basically means that one should never use a **raw pointer** to manage a resource. Therefore no destructor, copy constructor, copy assignment operator, move constructor and move assignment operator has to be implemented.

*The Rule of Zero* allows to implement **{C, M}**, **{UC, M}**, **{C, UM}** and **{UC, UM}**, **without declaring them explicitly**. The emphasized part of the last sentence is the important difference to *The Rule of Five*.

The aim of *The Rule of Zero* is to eliminate resource management for the user and let the Standard Library do all the work related to resource management.

The latest approved [ISO C++ Standard *C++14*](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4296.pdf) describes *The Rule of Zero* in the section 12.8 (“Copying and moving class objects”):



#### Suggestions

##### Use *Smart Pointers* instead of raw pointers:

[`std::unique_ptr `](http://en.cppreference.com/w/cpp/memory/unique_ptr) 

if an instance of a class can be moved, but not copied and does not have to be shared.

[`std::shared_ptr `](http://en.cppreference.com/w/cpp/memory/shared_ptr) 

if an instance of a class can be copied and has to be shared.

##### Avoid C-style language constructs (especially as class member attributes):

- The string class[`std::basic_string `](http://en.cppreference.com/w/cpp/string/basic_string)instead of a C-string (`char* `).
- The container class[`std::array `](http://en.cppreference.com/w/cpp/container/array)instead of C-arrays (e. g.`std::uint16_t my_var[10]; `).



```c++
/**
 * Demonstrates *The Rule of Zero* C++ idiom.
 *
 * @file the_rule_of_zero.h
 * @author Florian Wolters <wolters.fl@gmail.com>
 *
 * @section License
 *
 * Copyright Florian Wolters 2015 (http://blog.florianwolters.de).
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef FW_IDIOM_THE_RULE_OF_ZERO_H_
#define FW_IDIOM_THE_RULE_OF_ZERO_H_

#include <string>
#include <utility>

namespace fw {
namespace idiom {

/**
 * The TheRuleOfZero class demonstrates *The Rule of Zero* idiom.
 *
 * @author Florian Wolters <wolters.fl@gmail.com>
 */
class TheRuleOfZero final {
 public:
  /**
   * Initializes a new instance of the TheRuleOfZero class with the specified
   * string.
   *
   * @param kValue Thestring.
   */
  explicit TheRuleOfZero(std::string const& kValue) : value_{kValue} {
    // NOOP
  }

 private:
  /**
   * The value of this TheRuleOfZero instance.
   */
  std::string value_;
};

}  // namespace idiom
}  // namespace fw

#endif  // FW_IDIOM_THE_RULE_OF_ZERO_H_

/**
 * Runs the application.
 *
 * @return Always `0`.
 */
int main() {
  using fw::idiom::TheRuleOfZero;

  // Complete constructor.
  TheRuleOfZero the_rule_of_five_defaults{"hello, world"};

  // Copy constructor.
  TheRuleOfZero copy{the_rule_of_five_defaults};

  // Move constructor.
  TheRuleOfZero move{std::move(copy)};

  // Copy assignment operator.
  copy = the_rule_of_five_defaults;

  // Move assignment operator (from rvalue).
  move = TheRuleOfZero{"foo"};

  // Move assignment operator (from lvalue).
  move = std::move(copy);

  // Destructor(s).
}
```



## Summary

In my opinion, *The Rule of Zero* is currently the best practice for resource management if developing in the C++ language. It should be considered as the “modern” way, and all other approaches using manual resource management should be seen as “legacy” C-style language relicts.

By using automatic resource management, all required constructors and assignment operators can be implicitly declared and defined by the compiler.

> Always use automatic resource management in C++ by enforcing *The Rule of Zero*. Only use manual resource management (*The Rule of Three*, *The Rule of Five*) if the environment enforces it, e. g. due to a requirement.

