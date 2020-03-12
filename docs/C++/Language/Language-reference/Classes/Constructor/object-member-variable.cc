#include <iostream>
using namespace std;
class T
{
public:
    T()
    {
        a = 0;
        std::cout << "T default constructor" << std::endl;
    }
    T(int a) : a(a)
    {
        std::cout << "T constructor" << std::endl;
    }

private:
    int a;
};

class TT
{
public:
    TT()
    {
        std::cout << "TT default constructor" << std::endl;
    }
    TT(int a) : t(a)
    {
        std::cout << "TT constructor" << std::endl;
    }

private:
    T t;
};
int main()
{

    TT tt;
    TT tt2(1);
}