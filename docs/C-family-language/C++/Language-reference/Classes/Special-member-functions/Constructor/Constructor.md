# Constructor

“Constructor”即“构造函数”。我们已经知道了object是在runtime创建，根据Data-model章节的观点，我们可以将object简单地看做是一片内存区域，显然这片内存区域肯定会包含[non-static data members](https://en.cppreference.com/w/cpp/language/data_members)，那当程序运行时，如何来initialize这片内存区域呢？这对于c++ OOP是非常重要的，因为只有正确地初始化后，object才能够正确地运行。c++提供了丰富的constructor，程序员需要谨慎地实现以正确地initialize这些数据。需要注意的是，constructor是也是一种[Initialization](https://en.cppreference.com/w/cpp/language/initialization)。本文对c++中的constructor进行总结。



## cppreference [Non-static data members](https://en.cppreference.com/w/cpp/language/data_members)



## cppreference [Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/initializer_list)





## cppreference [Default constructors](https://en.cppreference.com/w/cpp/language/default_constructor)



## cppreference [Copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor)



## cppreference [Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor)



## cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)