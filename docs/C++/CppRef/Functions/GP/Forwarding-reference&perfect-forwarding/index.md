# Forwarding-reference&perfect-forwarding

C++11的 **forwarding reference** 是function generic programming的基础:

1. `auto&&`解决的是函数的返回值

2. `template &&`解决的函数的参数

由于上述两部分设计的内容较多，因此分开来进行描述。

