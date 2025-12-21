# Split coordinate

一、https://colab.research.google.com/drive/1BLQ8hipphX0aMUnp6sICbRCe6YE9mZBy?usp=sharing

二、[stackoverflow-How to draw polygons with Python?](https://stackoverflow.com/questions/43971259/how-to-draw-polygons-with-python) # [A](https://stackoverflow.com/a/43971350)

Using `matplotlib.pyplot`

```py
import matplotlib.pyplot as plt

coord = [[1,1], [2,1], [2,2], [1,2], [0.5,1.5]]
coord.append(coord[0]) #repeat the first point to create a 'closed loop'

xs, ys = zip(*coord) #create lists of x and y values

plt.figure()
plt.plot(xs,ys) 
plt.show() # if you need...
```