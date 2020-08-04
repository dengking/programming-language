# Double pointer

“double pointer”即“二级指针”

## Example

### [pthread_exit(3)](https://www.man7.org/linux/man-pages/man3/pthread_exit.3.html)  and [pthread_join(3)](https://www.man7.org/linux/man-pages/man3/pthread_join.3.html)

用户通过调用[`pthread_exit`](https://www.man7.org/linux/man-pages/man3/pthread_exit.3.html)来告知OS 本thread的返回值，即通过将返回值的地址传入到`pthread_exit`函数，所以`pthread_exit`的入参的类型是 `void *retval`，显然声明为`void *`是为了实现generic；显然在实现层，肯定会有一个global variable或者thread local variable来保存入参`retval`的值，这个variable应该是double pointer类型的，因为`pthread_exit`的入参类型是`void *`，所以这个variable如果声明为pointer类型的话，则会导致

pointer只能够直接point to object，所以它的使用方式:

```c
pointer = &object;
```

当入参的类型是pointer的时候，如果想要让另外一个pointer也指向这个pointer所指向的object，则需要将这个pointer声明为double pointer类型，在实现中，直接将pointer的值写入到这个double pointer，这样，这个double pointer就包含了和这个pointer相同的值，这样它就可以指向这个pointer指向的object了：

```c++
*double_pointer = pointer
```

可以这样认为：pointer的使用，则必然带来double pointer的使用。





即让`retval`指向返回值；[`pthread_join`](https://www.man7.org/linux/man-pages/man3/pthread_join.3.html) 要去取这个返回值，则需要使用一个pointer指向

让一个指针指向，则需要传入这个指针的地址，让后向这个地址中写入；

```

```



## TO READ

https://dev.to/noah11012/double-pointers-in-cc-2n96



https://beginnersbook.com/2014/01/c-pointer-to-pointer/



https://www.geeksforgeeks.org/double-pointer-pointer-pointer-c/



https://stackoverflow.com/questions/36689025/creating-double-pointer



good：非常好的类比

https://stackoverflow.com/questions/5580761/why-use-double-indirection-or-why-use-pointers-to-pointers/25110045