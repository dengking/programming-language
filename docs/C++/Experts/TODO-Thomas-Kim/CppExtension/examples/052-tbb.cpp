#include <iostream>
#include <mutex>
#include <tbb/tbb.h>
#include <tbb/concurrent_vector.h>


void test_tbb()
{
     tbb::task_scheduler_init init;

     std::mutex mutex;
     tbb::parallel_for(1, 10, [&mutex](auto& i)
     {
          std::lock_guard<std::mutex> lock(mutex);
          std::cout << i << std::endl;
     } );
}

template<typename Type>
Type serial_fibonacci_recursion(Type n)
{
     if (n < 2) // n = 0 or 1
          return n;
     else
          return serial_fibonacci_recursion(n-2) + serial_fibonacci_recursion(n-1);
}

template<typename Type>
Type serial_fibonacci_loop(Type n)
{
     if(n < 2)
          return n;
     else
     {
          Type fn_2 = 0;
          Type fn_1 = 1;
          Type fn;

          for(Type i=2; i <= n; ++i)
          {
               fn = fn_2 + fn_1;
               fn_2 = fn_1; fn_1 = fn;
          }

          return fn;
     }
          
}

void test_serial_fibonacci()
{
     for(int i=0; i < 10; ++i)
          std::cout << serial_fibonacci_loop(i) << std::endl;
}

void test_fibonacci_lambda()
{
     int fn_2 = 0;
     int fn_1 = 1;
     int fn;

     auto fibonacci = [&fn_2, &fn_1, &fn]()
     {
          fn = fn_2 + fn_1; 
          
          fn_2 = fn_1; fn_1 = fn;

          return fn;
     };

     std::cout << fn_2 << std::endl;
     std::cout << fn_1 << std::endl;
     
     for(int i=0; i < 10; ++i)
     {
          std::cout <<  fibonacci() << std::endl;
     }

}

const int CutOff = 16;
class FibTask: public tbb::task
{
     private:
          const long m_n;
          long* const m_sum;

          using base = tbb::task;

     public:
          FibTask(long n, long* sum): m_n{n}, m_sum{sum} { }

          tbb::task* execute() override
          {
               if(m_n < CutOff)
               {
                    *m_sum = serial_fibonacci_recursion(m_n);
               }
               else
               {
                    long x, y;
                    FibTask& a = *new(base::allocate_child()) FibTask(m_n-1, &x);
                    FibTask& b = *new(base::allocate_child()) FibTask(m_n-2, &y);

                    base::set_ref_count(3);
                    base::spawn(b);
                    base::spawn_and_wait_for_all(a);

                    *m_sum = x + y;
               }

               return nullptr;
          }
};

long ParallelFib(long n)
{
     long sum;
     FibTask& a = *new(tbb::task::allocate_root()) FibTask(n, &sum);
     
     tbb::task::spawn_root_and_wait(a);
     
     return sum;
}
/*

     (n, r) = (n-1, r-1) + (n-1, r)

     (4, 2) = (3, 1) + (3, 2)
          
 */

void test_parallel_fibonacci()
{
     for(long i=0; i < 10l; ++i)
          std::cout << ParallelFib(i) << std::endl;
}

int main()
{
     // test_tbb();
     
     // test_serial_fibonacci();
     // test_fibonacci_lambda();

     test_parallel_fibonacci();
}