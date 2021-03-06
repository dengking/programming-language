# Expression Template

在阅读[blaze](https://bitbucket.org/blaze-lib/blaze/src/master/)的文档时，其中介绍了*Smart Expression Template* ，Google了一下，发现 了c++中的expression template idiom。

## Expression Template and symbolic programming

关于Symbolic programming，参见`Theory\Programming-paradigm\Symbolic-programming`。Expression Template 可以看做是采用的 Symbolic programming的思想: 构建expression tree，关于这一点，在下面文章中有描述:

1) github mshadow [Expression Template Tutorial](https://github.com/dmlc/mshadow/blob/master/guide/exp-template/README.md)

2) mxnet [Deep Learning Programming Paradigm](https://mxnet.apache.org/versions/master/api/architecture/program_model)

## [More C++ Idioms/Expression-template](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Expression-template)

> NOTE: 这篇文章关于expression template的描述是非常好的。

**Domain-specific languages** (DSLs) is a way of developing programs where the problem to be solved is expressed using notation that is much closer to the domain of the problem rather than the usual notation (loops, conditionals, etc.) provided by **procedural languages**. **Domain-specific embedded languages** (DSELs) is a special case of DSLs where the notation is embedded in a host language (e.g., C++). Two prominent examples of DSELs based on C++ are the Boost Spirit Parser Framework1 and Blitz++2 scientific computing library. Spirit provides a notation to write EBNF grammar directly into a C++ program whereas Blitz++ allows a notation to perform mathematical operations on matrices. Obviously, such notation is not provided in C++ natively. The key benefit of using such notation is that the program captures the intention of the programmer quite intuitively making the program much more readable. It reduces the development as well as maintenance costs
dramatically.



## wikipedia [Expression templates](https://en.wikipedia.org/wiki/Expression_templates)

**Expression templates** are a [C++](https://en.wikipedia.org/wiki/C%2B%2B) [template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming) technique that builds structures representing a computation at compile time, where expressions are [evaluated only as needed](https://en.wikipedia.org/wiki/Lazy_evaluation) to produce efficient code for the entire computation.[[1\]](https://en.wikipedia.org/wiki/Expression_templates#cite_note-matsuzaki-1) Expression templates thus allow programmers to bypass the normal order of evaluation of the C++ language and achieve optimizations such as [loop fusion](https://en.wikipedia.org/wiki/Loop_fusion).

> NOTE: 这段总结非常好

### Applications

Outside of vector math, the [Spirit parser framework](https://en.wikipedia.org/wiki/Spirit_parser_framework) uses expression templates to represent [formal grammars](https://en.wikipedia.org/wiki/Formal_grammar) and compile these into parsers.

## TODO

1) modernescpp [Expression Templates](https://www.modernescpp.com/index.php/expression-templates)

2) flipcode [Faster Vector Math Using Templates](https://www.flipcode.com/archives/Faster_Vector_Math_Using_Templates.shtml)

## Implementation

### riptutorial [A basic example illustrating expression templates](https://riptutorial.com/cplusplus/example/19992/a-basic-example-illustrating-expression-templates)



```c++
#include <iostream>
#include <vector>
template <typename T, std::size_t COL, std::size_t ROW>
class Matrix {
public:
    using value_type = T;

    Matrix() : values(COL * ROW) {}

    static size_t cols() { return COL; }
    static size_t rows() { return ROW; }
	/**
	* x 行号
	* y 列号
	*/
    const T& operator()(size_t x, size_t y) const { return values[y * COL + x]; }
    T& operator()(size_t x, size_t y) { return values[y * COL + x]; }
    // 输出对象
	friend std::ostream& operator<<(std::ostream& OS, Matrix& M)
    {
        for(int i=0;i<ROW;++i)
        {
            for(int j=0;j<COL;++j)
            {
             	std::cout<<M(i, j);
            }
            std::cout<<std::endl;
        }
            
    }
private:
    std::vector<T> values;
};

template <typename T, std::size_t COL, std::size_t ROW>
Matrix<T, COL, ROW>
operator+(const Matrix<T, COL, ROW>& lhs, const Matrix<T, COL, ROW>& rhs)
{
    Matrix<T, COL, ROW> result;

    for (size_t y = 0; y != lhs.rows(); ++y) {
        for (size_t x = 0; x != lhs.cols(); ++x) {
            result(x, y) = lhs(x, y) + rhs(x, y);
        }
    }
    return result;
}

int main()
{
    const std::size_t cols = 2000;
    const std::size_t rows = 1000;

    Matrix<double, cols, rows> a, b, c;

    // initialize a, b & c
    for (std::size_t y = 0; y != rows; ++y) {
        for (std::size_t x = 0; x != cols; ++x) {
            a(x, y) = 1.0;
            b(x, y) = 2.0;
            c(x, y) = 3.0;
        }
    }  

    Matrix<double, cols, rows> d = a + b + c;  // d(x, y) = 6 
    std::cout<<d;
}

```

