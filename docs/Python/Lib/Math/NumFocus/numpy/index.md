# [NumPy](https://numpy.org/)



## [NumPy quickstart](https://numpy.org/doc/stable/user/quickstart.html)



### [The Basics](https://numpy.org/doc/stable/user/quickstart.html#the-basics)

NumPy’s main object is the homogeneous multidimensional array. It is a table of elements (usually numbers), all of the same type, indexed by a tuple of non-negative integers. In NumPy dimensions are called *axes*.

For example, the array for the coordinates of a point in 3D space, `[1, 2, 1]`, has one axis. That axis has 3 elements in it, so we say it has a length of 3. In the example pictured below, the array has 2 axes. The first axis has a length of 2, the second axis has a length of 3.

```python
[[1., 0., 0.],
 [0., 1., 2.]]
```

> NOTE:
>
> 一、axis的中文意思是"轴"，但是numpy中的axis和我们平时所说的空间直角坐标系的轴并不能够等价；在`numpy`中，numpy中的 `ndarray` 是以"array of array"、nested、recursive的方式定义的，这种定义方式是符合普遍的认知的，关于此可以参见"Multidimensional-array"，在numpy中，一个axis对应一层pack，一个axi对应一个dimension 
>
> 我们常说的n-维数组，其实就对应了numpy的 `ndarray` 
>
> draft: 关于numpy的axis的理解，可以参考《math-tensor-matrix-and-vector.md》

NumPy’s array class is called `ndarray`. It is also known by the alias `array`. Note that `numpy.array` is not the same as the Standard Python Library class `array.array`, which only handles one-dimensional arrays and offers less functionality. The more important attributes of an `ndarray` object are:

#### `ndarray.ndim`

the number of axes (dimensions) of the array.

#### `ndarray.shape`

the dimensions of the array. This is a tuple of integers indicating the size of the array in each dimension. For a matrix with *n* rows and *m* columns, `shape` will be `(n,m)`. The length of the `shape` tuple is therefore the number of axes, `ndim`.

#### `ndarray.size`

the total number of elements of the array. This is equal to the product of the elements of `shape`.

#### `ndarray.dtype`

an object describing the type of the elements in the array. One can create or specify dtype’s using standard Python types. Additionally NumPy provides types of its own. numpy.int32, numpy.int16, and numpy.float64 are some examples.

#### `ndarray.itemsize`

the size in bytes of each element of the array. For example, an array of elements of type `float64` has `itemsize` 8 (=64/8), while one of type `complex32` has `itemsize` 4 (=32/8). It is equivalent to `ndarray.dtype.itemsize`.

#### `ndarray.data`

the buffer containing the actual elements of the array. Normally, we won’t need to use this attribute because we will access the elements in an array using indexing facilities.





## 如何理解along axis

正如在numpy的Quickstart tutorial[¶](https://www.numpy.org/devdocs/user/quickstart.html#quickstart-tutorial)中所介绍的：

> NumPy’s array class is called `ndarray`.

即n维数组；

numpy提供了一系列的api提供了`axis`入参的，比如：

*   [numpy.amax](https://docs.scipy.org/doc/numpy/reference/generated/numpy.amax.html)

在这些api的doc中都会有这样的描述：

> along an axis

比如[numpy.amax](https://docs.scipy.org/doc/numpy/reference/generated/numpy.amax.html)的doc中有这样的描述：

> Return the maximum of an array or maximum along an axis.

那我应该如何来理解along an axis的含义呢？

下面以[numpy.amax](https://docs.scipy.org/doc/numpy/reference/generated/numpy.amax.html)为例来探索numpy中**along an axis** 的含义。

    a = np.array([[[0, 1, 2], [3, 4, 5]],[[6, 7, 8], [9, 10, 11]]])
    
    a.shape
    Out[32]: (2, 2, 3)

对上述`ndarray a`如果按照空间直角坐标系中（x轴对应于axis-0，y轴对应axis-1，z轴对应axis-2）来进行描述的话，则它可以按照如下的方式来进行摆放：

    图略



    np.max(a, axis=0)
    Out[33]: 
    array([[ 6,  7,  8],
           [ 9, 10, 11]])

`np.max(a, axis=0)`的含义是沿着axis-0，取所有元素中的最大值，从返回的结果来看， 我可以按照如下的方式来进行理解：

### 理解方式一

`a`在axis-0轴上有2个元素（`a.shape=(2, 2, 3)`），这两个元素分布是：

    a[0]
    Out[36]: 
    array([[0, 1, 2],
           [3, 4, 5]])



    a[1]
    Out[37]: 
    array([[ 6,  7,  8],
           [ 9, 10, 11]])

沿着axis-0轴取max元素意味着，对`a[0]`中的元素都和`a[1]`中的元素执行element-wise的比较，如下列举了一些例子：

*   `max(a[0][0][0], a[1][0][0])`
*   `max(a[0][0][1], a[1][0][1])`

如果从空间的角度来看待这个问题的话，其实它就类似于沿着axis-0轴射来一道光，沿着这道光穿起来的元素进行`max`。





## [NumPy reference#Routines](https://docs.scipy.org/doc/numpy/reference/routines.html#routines)



### [Array manipulation routines](https://docs.scipy.org/doc/numpy/reference/routines.array-manipulation.html#array-manipulation-routines) 



### [Joining arrays](https://docs.scipy.org/doc/numpy/reference/routines.array-manipulation.html#joining-arrays) 



|                                                              |                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------- |
| [`concatenate((a1, a2, …)[, axis, out])`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.concatenate.html#numpy.concatenate) | Join a sequence of arrays along an existing axis.       |
| [`stack(arrays[, axis, out])`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.stack.html#numpy.stack) | Join a sequence of arrays along a new axis.             |
| [`column_stack(tup)`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.column_stack.html#numpy.column_stack) | Stack 1-D arrays as columns into a 2-D array.           |
| [`dstack(tup)`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.dstack.html#numpy.dstack) | Stack arrays in sequence depth wise (along third axis). |
| [`hstack(tup)`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.hstack.html#numpy.hstack) | Stack arrays in sequence horizontally (column wise).    |
| [`vstack(tup)`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.vstack.html#numpy.vstack) | Stack arrays in sequence vertically (row wise).         |
| [`block(arrays)`](https://docs.scipy.org/doc/numpy/reference/generated/numpy.block.html#numpy.block) | Assemble an nd-array from nested lists of blocks.       |



#### [numpy.stack](https://docs.scipy.org/doc/numpy/reference/generated/numpy.stack.html#numpy-stack)

`numpy.stack(arrays, axis=0, out=None)`**¶**  \[[source\]](https://github.com/numpy/numpy/blob/v1.17.0/numpy/core/shape_base.py#L356-L432)

Join a sequence of arrays along a new axis.

The `axis` parameter specifies the index of the **new axis** in the dimensions of the result. For example, if `axis=0` it will be the first dimension and if `axis=-1` it will be the last dimension.

##### Examples

```python
arrays = [np.random.randn(3, 4) for _ in range(10)]
np.stack(arrays, axis=0).shape

```

输出为：`(10, 3, 4)`

如果从空间进行想象的话，上述操作实际是将一个形状为`[3,4]`的矩形验证axis-0进行滑动，共滑动10步；

```python
np.stack(arrays, axis=1).shape
```

输出为：`(3, 4, 10)`

如果从空间进行想象的话，上述操作实际是将一个形状为`[3,4]`的矩形验证axis-2进行滑动，共滑动10步；

    >>> a = np.array([1, 2, 3])
    >>> b = np.array([2, 3, 4])
    >>> np.stack((a, b))
    array([[1, 2, 3],
           [2, 3, 4]])



    >>> np.stack((a, b), axis=-1)
    array([[1, 2],
           [2, 3],
           [3, 4]])

沿着哪一轴进行stack，那么这一轴的元素的个数就是被堆叠的元素的个数；



|          | `numpy.stack`                                                | `numpy.concatenate`                                          |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 空间操作 | 堆叠；<br />直观的想象是把盘子堆叠起来、把书水平地码放到书架中；<br />堆叠起来的array的维度会比原array的维度多一 | 拼接；<br />直观的想象就是使用胶带将多张纸粘连成一张更大的纸；<br />拼接而成的array和原array的维度相同 |
|          |                                                              |                                                              |
|          |                                                              |                                                              |

### Split array



#### [numpy.split](https://numpy.org/doc/stable/reference/generated/numpy.split.html) 



> NOTE:
>
> 一、stackoverflow [Easiest way to split 2d numpy array in to two 1d arrays?](https://stackoverflow.com/questions/66327451/easiest-way-to-split-2d-numpy-array-in-to-two-1d-arrays) # [A](https://stackoverflow.com/a/66327490)
>
> ```python
> import numpy as np
> arr=np.array([[0,1],[2,3],[4,5],[6,7],[8,9],[10,11]])
> arr1,arr2=np.split(arr,2,axis=1)
> ```
>
> ```
> arr1
> Out[21]: 
> array([[ 0],
>        [ 2],
>        [ 4],
>        [ 6],
>        [ 8],
>        [10]])
> 
> arr2
> Out[22]: 
> array([[ 1],
>        [ 3],
>        [ 5],
>        [ 7],
>        [ 9],
>        [11]])
> ```
>
> 



### Flat array

一、geeksforgeeks [Python | Flatten a 2d numpy array into 1d array](https://www.geeksforgeeks.org/python-flatten-a-2d-numpy-array-into-1d-array/)

Method #1 : Using [np.flatten()](https://numpy.org/doc/stable/reference/generated/numpy.ndarray.flatten.html) 

Method #2: Using [np.ravel()](https://numpy.org/doc/stable/reference/generated/numpy.ravel.html) 

Method #3: Using [np.reshape()](https://numpy.org/doc/stable/reference/generated/numpy.reshape.html) 

二、[stackoverflow-From ND to 1D arrays](https://stackoverflow.com/questions/13730468/from-nd-to-1d-arrays)



## Examples



### scatter points

https://colab.research.google.com/drive/1BLQ8hipphX0aMUnp6sICbRCe6YE9mZBy?usp=sharing

