首先写一个能够编译通过的版本，如下：
```
#include<iostream>
using namespace std;
  
class point {
private:
    int x, y;
public:
    static int count;
    point(int xx, int yy) :x(xx), y(yy) { count++; }
    point() { count++; }
};
int point::count = 1;//count在class point中声明，在此进行定义

class ArrPoint
{
private:
    int size;
    point* p;
  
public:
    ArrPoint(int s)
    {
        p = new point[s];
        size = s;
    }
    point& operator [](int i)//只有入参
    {
        if(i >= size)
        {
            // 越界处理
        }
        return p[i];
    }
};
  
int main()
{
    ArrPoint a(5);
    a[0], a[1], a[2], a[3], a[4];
  
  
    return 0;
}
```
编译脚本如下：
```
g++ overload.cpp -o overload
```
按照上面的写法，是能够编译通过的，

对上述程序进行修改，将operator[]设置为有两个入参，如下：
```
    point& operator [](int i, int j)
    {
        if(i*j >= size)
        {
            // 瓒𸐿𻃣澶𶰿𹳝
        }
        return p[i];
    }
```
再进行编译，则会报下列错误:
```
overload.cpp:27:36: 错误：‘point& ArrPoint::operator[](int, int)’带且仅带 1 个实参
     point& operator [](int i, int j)
overload.cpp: 在函数‘int main()’中:
overload.cpp:40:6: 错误：no match for ‘operator[]’ (operand types are ‘ArrPoint’ and ‘int’)
     a[0, 1], a[1, 1], a[2, 1], a[3, 1], a[4, 1];
```
错误原因分析：

[]是一元运算符，所以c++限制在对其进行重载的时候，只能够有一个入参（从表面来看，这段话是说得通的，实际上是存在问题的：operator[]的入参包括类本身和另外一个声明的参数）。

这告诉我，重载运算符的时候，需要注意这个运算符是几元运算符，对应的重载函数才能够有几个入参。
