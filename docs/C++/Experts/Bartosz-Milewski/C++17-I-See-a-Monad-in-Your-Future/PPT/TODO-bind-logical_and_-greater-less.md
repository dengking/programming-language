```c++
v.erase(remove_if(v.begin(), v.end(),
                  bind(logical_and<bool>(),
                       bind(greater<int>(), _1, -10),
                       bind(less<int>(), _1, 10))),
        v.end());

```

在PPT中，给出了上述代码。



stackoverflow [use std::logical_and to combine two conditions](https://stackoverflow.com/questions/10284007/use-stdlogical-and-to-combine-two-conditions)

https://en.cppreference.com/w/cpp/utility/functional/bind

https://en.cppreference.com/w/cpp/utility/functional/logical_and

https://en.cppreference.com/w/cpp/utility/functional/logical_and_void