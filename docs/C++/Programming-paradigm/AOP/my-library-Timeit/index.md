# Timeit

一、度量函数的耗时，是各种programming language中的一个任务，本文描述C++中如何来实现这个需求。

二、参考如下:

1、stackoverflow [Measuring execution time of a function in C++](https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c)

2、stackoverflow [How to Calculate Execution Time of a Code Snippet in C++](https://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c) 

3、stackoverflow [how to write a C++ class member function wrapper?](https://stackoverflow.com/questions/48355723/how-to-write-a-c-class-member-function-wrapper)

4、

https://en.cppreference.com/w/cpp/utility/functional/bind

https://en.cppreference.com/w/cpp/utility/functional/mem_fn



## 我的实现



参见当前目录的`func_timer.h`



## RAII

### stackoverflow [Cost of copy vs move std::shared_ptr](https://stackoverflow.com/questions/70053971/cost-of-copy-vs-move-stdshared-ptr)



#### [A](https://stackoverflow.com/a/70054410/10173843)



I wrote a benchmark. On my Macbook Air it is three times faster (`g++` as well as `clang++` `-std=c++17 -O3 -DNDEBUG`). Let me know if you see problems with the benchmark.

```cpp
#include <chrono>
#include <iostream>
#include <vector>
#include <memory>
using namespace std;
using namespace std::chrono;

int COUNT = 50'000'000;

struct TimeIt
{
    system_clock::time_point start;
    TimeIt()
    {
        start = system_clock::now();
    }
    ~TimeIt()
    {
        auto runtime = duration_cast<milliseconds>(system_clock::now() - start).count();
        cout << runtime << " ms" << endl;
    }
};

void benchmark_copy(const vector<shared_ptr<int>> &vec_src)
{
    cout << "benchmark_copy" << endl;
    vector<shared_ptr<int>> vec_dst;
    vec_dst.reserve(COUNT);
    TimeIt ti;
    for (auto &sp : vec_src)
        vec_dst.emplace_back(sp);
}
void benchmark_move(vector<shared_ptr<int>> &&vec_src)
{
    cout << "benchmark_move" << endl;
    vector<shared_ptr<int>> vec_dst;
    vec_dst.reserve(COUNT);
    TimeIt ti;
    for (auto &sp : vec_src)
        vec_dst.emplace_back(move(sp));
}

int main(int arg, char **argv)
{

    vector<shared_ptr<int>> vec;
    for (int i = 0; i < COUNT; ++i)
        vec.emplace_back(new int);

    benchmark_copy(vec);
    benchmark_move(move(vec));
}

```