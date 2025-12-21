# Matplotlib



## [Quick start guide](https://matplotlib.org/stable/users/explain/quick_start.html) 

> NOTE:
>
> 一、介绍Matplotlib的基本概念



### Types of inputs to plotting functions

Plotting functions expect [`numpy.array`](https://numpy.org/doc/stable/reference/generated/numpy.array.html#numpy.array) or [`numpy.ma.masked_array`](https://numpy.org/doc/stable/reference/generated/numpy.ma.masked_array.html#numpy.ma.masked_array) as input, or objects that can be passed to [`numpy.asarray`](https://numpy.org/doc/stable/reference/generated/numpy.asarray.html#numpy.asarray). 

> NOTE:
>
> 一、Matplotlib是基于 [`numpy.array`](https://numpy.org/doc/stable/reference/generated/numpy.array.html#numpy.array) 而设计的





## [Plot types](https://matplotlib.org/stable/plot_types/index.html)

> NOTE:
>
> 一、 matplotlib能够画哪些类型的图，这类进行了非常好的说明

### Pairwise data



### [scatter(x, y)](https://matplotlib.org/stable/plot_types/basic/scatter_plot.html)

> NOTE:
>
> 一、"scatter"即"散点图"，这种图的一些例子:
>
> 1、[LeetCode-587. Erect the Fence](https://leetcode.cn/problems/erect-the-fence/) 



```python
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('_mpl-gallery')

# make the data
np.random.seed(3)
x = 4 + np.random.normal(0, 2, 24)
y = 4 + np.random.normal(0, 2, len(x))
# size and color:(设置每个点的size、color)
sizes = np.random.uniform(15, 80, len(x))
colors = np.random.uniform(15, 80, len(x))

# plot
fig, ax = plt.subplots()

ax.scatter(x, y, s=sizes, c=colors, vmin=0, vmax=100)

ax.set(xlim=(0, 8), xticks=np.arange(1, 8),
       ylim=(0, 8), yticks=np.arange(1, 8))

plt.show()
```



### Statistical distributions

### Gridded data:

### Irregularly gridded data

### 3D and volumetric data





## Draft

### _tkinter.TclError: no display name and no $DISPLAY environment variable

https://stackoverflow.com/questions/2801882/generating-a-png-with-matplotlib-when-display-is-undefined

上面这篇文章虽然给出了答案，但是原因并不清楚，下面这篇文章给出了答案，并且还给出了原因：

https://stackoverflow.com/questions/4931376/generating-matplotlib-graphs-without-a-running-x-server

>Matplotlib seems to require the $DISPLAY environment variable which means a running X server.
>Some web hosting services do not allow a running X server session.
>Is there a way to generate graphs using matplotlib without a running X server?

它的这段话是对我的这个问题更好，更加专业的描述。

### Setting different color for each series in scatter plot on matplotlib

https://stackoverflow.com/questions/12236566/setting-different-color-for-each-series-in-scatter-plot-on-matplotlib

### Named colors in matplotlib

https://stackoverflow.com/questions/22408237/named-colors-in-matplotlib

### matplotlib如何画出pandas的索引和列之间的关系图？？

下面是一个小例子：

```
%pylab inline
ts = pd.Series(np.random.randn(1000), index=pd.date_range('1/1/2000',periods=1000))
ts = ts.cumsum()
ts.plot()
```

从画出的图像来看，它的确能够以索引为`x`轴，以对应的值为`y`轴，画出两者之间的关系，其实对于一个DataFrame，其索引和每一列的值之间是一一对应的函数关系，这种对应关系用上面的图像来表示就是一个一个的点。

也就是如果调用`matplotlib`的plot函数来画一个`Line Plots`的话，前提要求应该是`x`和`y`的长度需要是相同的，`matplotlib`使用`[x,y]`来构成所有的点集，然后将这些点画出，这应该就是matplotlib的工作原理了。

```
plot(x, y)
```

