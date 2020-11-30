
示例代码如下：
```C++
#include<iostream>
using namespace std;
class Test {
    friend ostream & operator<<(ostream &out, Test &obj);
    friend istream & operator >> (istream &in, Test &obj);
public:
    Test(int a = 0, int b = 0)
    {
        this->a = a;
        this->b = b;
    }
    void display()
    {
        cout << "a:" << a << " b:" << b << endl;
    }
private:
    int a;
    int b;
};
ostream & operator<<(ostream &out, Test &obj)
{
    out << obj.a << " " << obj.b;
    return out;
}
istream & operator>>(istream &in, Test &obj)
{
    in >> obj.a>> obj.b;
    if (!in)
    {
        obj = Test();
    }
    return in;
}
int main()
{
    Test t1(1, 2);
    cout << t1 << endl;
    cout << "请输入两个int属性:";
    cin >> t1;
    cout << t1 << endl;;
    cout << "hello world!\n";
    return 0;
}
```
如果是重载双目操作符（比如+，=等等，一般将其定义为类的成员函数），就只要设置一个参数作为**右侧运算量**，而左侧运算量就是对象本身。

而 >>  或<< 左侧运算量是cin或cout，而不是对象本身，所以不满足后面一点，就只能申明为友元函数了。

如果一定要声明为成员函数，只能成为如下的形式：

```
ostream & operator<<(ostream &output)
{
　　return output;
}
```
所以在运用这个<<运算符时就变为这种形式了：
```
data<<cout;
```
不合符人的习惯。

在重载输出输入运算符的时候，只能采用**全局函数**的方式（因为我们不能在ostream和istream类中编写成员函数），这里才是**友元函数**真正的应用场景。对于输出运算符，主要负责打印对象的内容而非控制格式，输出运算符不应该打印换行符；对于输入运算符，必须处理可能失败的情况（通常处理输入失败为默认构造函数的形式），而输出运算符不需要。