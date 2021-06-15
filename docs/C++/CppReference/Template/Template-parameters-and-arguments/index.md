# 关于本章

本章对C++的Template parameters and template arguments进行讨论。

## Static、compile-time

C++ template是典型的compile-time concept，[instantiation of  template](https://en.cppreference.com/w/cpp/language/templates) 是发生在compile-time的，所以compiler必须要在compile-time获知它的argument，对于type template parameter，这是非常容易的，因为type往往是static的；对于non-type template parameter，compiler就需要考虑它value是否能够compile-time得到。

