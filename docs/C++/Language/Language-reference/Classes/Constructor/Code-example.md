# Code example



## How can I initialize C++ object member variables in the constructor?

参见[How can I initialize C++ object member variables in the constructor?](https://stackoverflow.com/questions/12927169/how-can-i-initialize-c-object-member-variables-in-the-constructor)的[回答](https://stackoverflow.com/a/12927220)。



```c++
#include <iostream>
using namespace std;
class T{
public:
T(){
a = 0;
std::cout<<"T default constructor"<<std::endl;
}
T(int a) : a(a){
std::cout<<"T constructor"<<std::endl;
}
private:
int a;
}

class TT{
public:
TT(){
std::cout<<"TT default constructor"<<std::endl;
}
TT(int a): t(a){
std::cout<<"TT constructor"<<std::endl;
}
private:
T t;
}
int main(){

TT tt;
TT tt2(1);
}
```

