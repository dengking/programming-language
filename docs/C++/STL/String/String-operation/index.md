# 关于本章

保障描述string的常见操作的实现，这些常见操作伴随着我们的开发过程。

## trim

[What's the best way to trim std::string?](https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring)

[HOW TO TRIM A STD::STRING IN C++](http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html)



## char to string

https://stackoverflow.com/questions/1195675/convert-a-char-to-stdstring

https://www.techiedelight.com/convert-char-to-string-cpp/

https://www.geeksforgeeks.org/convert-character-array-to-string-in-c/



## c string to `std::string`

```C++
std::string  pchar_to_string(const char * pchar)
{
    if (NULL == pchar) return "";

    return std::string(pchar); 
}
```



## C string operation 参考

对于C string，需要对它的常用算法进行总结，下面是可以参考的内容:

1) [ext/misc/csv.c](https://sqlite.org/src/finfo?name=ext/misc/csv.c&m&ci=tip) 

