#include <iostream>
#include <vector>
#include <initializer_list>
#include <cstddef>
#include "test.h"
#include <type_traits>
template<class S>
class C
{
public:
C(std::initializer_list<S> l):m_s(l)
{
m_c = l.size();
} 
int m_c;
std::vector<S> m_s;
};

struct Test
{
public:
Test(int i):m_i(i){}
int m_i;
};

int main(){
C<Test> c = {1, 2, 3, 4, 5};
std::cout<<c.m_c<<std::endl;
}
