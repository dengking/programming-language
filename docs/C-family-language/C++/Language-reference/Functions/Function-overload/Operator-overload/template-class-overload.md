# **模板类**中重载<<和>>

在模板类中**输入流**>>和**输出流**<<的重载,若使用友元在类内声明，在类外实现，那么连接时将会报错,但我们可以采用以下三种方式来实现**输出流**<<和**输入流**>>的重载。

##  方法一：

将输出流<<和输入流>>重载的实现写在类中


```c++
#include   <iostream>
using   namespace   std;  

template<class T>  
class Test  
{  
   public:  
Test(const T& t):data(t){}
//---------------------------------------------
friend ostream& operator<<(ostream& out,Test<T>& t)    //输出流重载声明及实现
{
     return out<<"data   is   "<<t.data;
} //--------------------------------------------
friend istream& operator>>(istream& in,Test<T>& t)      //输入流重载声明及实现
{
    return in>>t.data;
}//---------------------------------------------
   private:  
T data;  
};//-----------------------------------------------------------------      
int   main()  
{  
   Test<int> b(3);  
   cout<<b<<'n';  
   cin>>b;
   cout<<b<<'n';
   return 0;
}
```


那么为什么模板类的**输入输出流重载**不能在类内声明，类外实现呢？？因为模板比较特殊，如果在模板类外实现重载的话则写法如下：
```c++
template<class T>
ostream& operator<<(ostream& out,Test<T>& t)
{
  return out<<"data   is   "<<t.data;
} //--------------------------------------------
```
上面正好是**函数模板**的定义,而我们知道**操作符重载函数**不是类的成员函数，因此此处相当于定义了一个新的函数模板（不同于类中的`friend ostream& operator<<(ostream& out,Test<T>& t)）`。但若去掉`template<class T> `，函数中的参数`Test<T>`就不知是什么类型，所以不能在模板类内声明，类外实现操作符重载。

## 方法二

既然类外实现相当于重定义了一个**函数模板**，那么只要他不使用类的**私用成员**即可，因此重载的函数模板只有通过类的**公有成员函数**来实现对类的**私有成员**的操作，这样不必在类内声明它为**友元**，直接在类外**重载**即可。


```c++
#include "stdafx.h"
#include   <iostream>
using   namespace   std;  
template<class T>  
class Test  
{  
   public:  
Test(const T& t):data(t){}
T GetData()const{return data;}
void SetData(T &item){data=item;}
   private:  
T data;  
};//-----------------------------------------------------------------
template<class T>      
ostream& operator<<(ostream& out,Test<T>& t)   
{
return out<<"data   is   "<<t.GetData();
} //--------------------------------------------
template<class T>
istream& operator>>(istream& in,Test<T>& t)     
{
 T item;
in>>item;
t.SetData(item);
return in;
}//---------------------------------------------      
int   main()  
{  
   Test<int> b(3);  
   cout<<b<<'n';  
   cin>>b;
   cout<<b<<'n';
   return 0;
} 
```

## 方法三

使用过渡函数


```c++
#include "stdafx.h"
#include   <iostream>
using   namespace   std;  
template<class   T>  
class   Test  
{  
   public:  
      Test(const T& t):data(t){}
      //---------------------------------------------
      template<class CharT,class CharTraits>  
      basic_ostream<CharT,CharTraits>& Output(basic_ostream<CharT,CharTraits>& out)const    //输出流过渡函数
      {
            return out<<"data   is   "<<data;
      } //--------------------------------------------
      template<class   CharT,class   CharTraits>  
      basic_istream<CharT,CharTraits>& Input(basic_istream<CharT,CharTraits>& in)    //输入流过渡函数
      {
          return in>>data;
      }//---------------------------------------------
   private:  
      T data;  
};//-----------------------------------------------------------------      
template<class T,class CharT,class CharTraits>  
basic_ostream<CharT,CharTraits>& operator<<(basic_ostream<CharT,CharTraits>& out,const Test<T>& t)   //输出流重载
{  
    return t.Output(out);  
}//------------------------------------------------------------------      
template<class T,class CharT,class CharTraits>  
basic_istream<CharT,CharTraits>& operator>>(basic_istream<CharT,CharTraits>& in,Test<T>& t)   //输入流重载
{  
    return t.Input(in);  
}//------------------------------------------------------------------      
int   main()  
{  
   Test<int>   b(4);  
   cout<<b<<'n';  
   cin>>b;
   cout<<b<<'n';
   return 0;
}
```

## 方法四

https://blog.csdn.net/u010003835/article/details/47312955

这种方法不行

```c++
#include<iostream>  
using namespace std;  
template <class T>  
class Test  
{  
private:  
   T num;  
public:  
 Test( T n)  
 {  
  num=n;  
 }  
friend ostream& operator<< <>(ostream &out,const Test<T> &);  
};  
template <class T>  
ostream& operator<< <>(ostream &out,const Test<T> &obj)  
{  
 out<<obj.num;  
 return out;  
};  
void main()  
{  
 Test<int> t(414);  
 cout<<t;  
} 
```

## 方法五

https://blog.csdn.net/u010003835/article/details/47312955

```c++
#include <iostream>  
  
using namespace std;  
  
template<class T>  
  
class Test  
  
{  
  
public:  
  
    template<class S>                   // 注意<class S>S  
    friend ostream& operator << (ostream& out, const Test<S>& obj);  
  
  
    Test(T n = 0) :num(n) {}  
  
  
  
    Test(const Test<T>& copy)  
    {  
        num = copy.num;  
    }  
  
  
  
private:  
  
    T num;  
  
};  
  
  
// 注意<class T>和<class S>都行  
  
template<class T>  
ostream& operator << (ostream& out, const Test<T>& obj)  
{  
    out << obj.num << endl;  
  
    return out;  
}  
  
  
int main(){  
    Test<int> t(2);  
  
    cout << t;  
  
    return 0;  
}  
```


