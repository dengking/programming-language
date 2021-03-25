# 判断map中是否包含key/in

这是在阅读 labuladong [我写了套框架，把滑动窗口算法变成了默写题](https://mp.weixin.qq.com/s/ioKXTMZufDECBUwRRp3zaA) 时看到的，其中使用的是count

## stackoverflow [Determine if map contains a value for a key?](https://stackoverflow.com/questions/3136520/determine-if-map-contains-a-value-for-a-key)



### [A](https://stackoverflow.com/a/3136537)

> NOTE: 
>
> 1、使用`find`

> Does something along these lines exist?

No. With the stl map class, you use [`::find()`](http://en.cppreference.com/w/cpp/container/map/find) to search the map, and compare the returned iterator to `std::map::end()`

so

```
map<int,Bar>::iterator it = m.find('2');
Bar b3;
if(it != m.end())
{
   //element found;
   b3 = it->second;
}
```

Obviously you can write your own `getValue()` routine if you want (also in C++, there is no reason to use `out`), but I would suspect that once you get the hang of using `std::map::find()` you won't want to waste your time.

**Also your code is slightly wrong:**

`m.find('2');` will search the map for a keyvalue that is `'2'`. IIRC the C++ compiler will implicitly convert '2' to an int, which results in the numeric value for the ASCII code for '2' which is not what you want.

Since your keytype in this example is `int` you want to search like this: **`m.find(2);`**



### [A](https://stackoverflow.com/a/11765524)

> NOTE: 
>
> 1、使用`count`

As long as the map is not a multimap, one of the most elegant ways would be to use the count method

```
if (m.count(key))
    // key exists
```

The count would be 1 if the element is indeed present in the map.

## stackoverflow [How to find if a given key exists in a C++ std::map](https://stackoverflow.com/questions/1939953/how-to-find-if-a-given-key-exists-in-a-c-stdmap)