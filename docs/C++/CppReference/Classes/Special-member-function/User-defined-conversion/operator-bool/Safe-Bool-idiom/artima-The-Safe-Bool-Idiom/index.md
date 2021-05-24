# artima [The Safe Bool Idiom](https://www.artima.com/cppsource/safebool.html)

Learn how to validate objects in a boolean context without the usual harmful side effects.

**I**n C++, there are a number of ways to provide Boolean tests for classes. Such support is either provided to make usage intuitive, to support **generic programming**, or both. We shall examine four popular ways of adding support for the popular and idiomatic `if (object) {}` construct. To conclude, we will discuss a new solution, *without the pitfalls and dangers of the other four*. Let the games begin.

