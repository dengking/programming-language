# Temporary Base Class



## More C++ Idioms/[Temporary Base Class](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Base_Class)

### Intent

Reduce the cost of creating temporary objects.

### Motivation

**Temporary objects** are often created during execution of a C++ program. Result of C++ operators (unary, binary, logical, etc.) and return-by-value functions always give rise to temporary objects. For built-in types, the cost of creating temporaries is minimal because compilers often use CPU registers to manipulate them. However, creation of temporary objects can be quite expensive for user-defined classes that allocate memory in the constructor and may require expensive copy operations in the copy-constructor. Temporaries are often wasteful because their lifespan is usually very short and they exist only to be assigned to a named object (lvalue). Even though their lifespan is short, the C++ language rules require the compilers to create and destroy temporaries to maintain correctness of the program. (In reality, RVO and NRVO optimizations are allowed to eliminate some temporaries). The cost of creating and destroying **temporaries** can adversely affect the performance of programs that use heavy objects.



```c++
#include<algorithm>
#include<iostream>
using namespace std;
void do_addition(int * dest, const int * src1, const int * src2, size_t dim)
{
  for(size_t i = 0; i < dim * dim; ++i, ++dest, ++src1, ++src2)
    *dest = *src1 + *src2;
}

class Matrix
{
  size_t dim;
  int * data;

public:

  explicit Matrix(size_t d)
    : dim(d), data(new int[dim*dim]())
  {
    for(size_t i = 0;i < dim * dim; ++i)
      data[i] = i*i;  
    cout<<"ctor"<<std::endl;
  }
  
  Matrix(const Matrix & m)
    : dim(m.dim), data(new int[dim*dim]())
  {
    std::copy(m.data, m.data + (dim*dim), data);
  }
  
  Matrix & operator = (const Matrix & m)
  {
    Matrix(m).swap(*this);
    return *this;
  }
  
  void swap(Matrix & m)
  {
    std::swap(dim, m.dim);
    std::swap(data, m.data);
  }
  
  Matrix operator + (const Matrix & m) const
  {
    Matrix result(this->dim);
    do_addition(result.data, this->data, m.data, dim);
    return result;
  }

  ~Matrix()
  {
    delete [] data;
    cout<<"dctor"<<std::endl;
  }
};

int main()
{
Matrix m1(3), m2(3), m3(3), m4(3);
Matrix result = m1 + m2 + m3 + m4;
}
```



Using objects of this class in expression such as below have several performance issues.

```c++
Matrix m1(3), m2(3), m3(3), m4(3);
Matrix result = m1 + m2 + m3 + m4;
```

> NOTE: 上述程序的运行结果如下：
>
> ```
> ctor
> ctor
> ctor
> ctor
> ctor
> ctor
> ctor
> dctor
> dctor
> dctor
> dctor
> dctor
> dctor
> dctor
> ```
>
> 可以看能到，创建和销毁了多个temporary

Temporary `Matrix` objects are created as a result of every summation and they are destroyed immediately. For every pair of parenthesis in `(((m1 + m2) + m3) + m4)`, a temporary object is needed. Creation and destruction of each temporary requires memory allocation and de-allocation, which is quite wasteful.

Temporary Base Class idiom is a way of reducing the overhead of temporaries in arithmetic expression such as the above. However, this idiom has major drawbacks as described towards the end.

