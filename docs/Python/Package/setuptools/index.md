# Setuptools

一、build wheel

```shell
python setup.py bdist_wheel
```

参考自: stackoverflow [Build a wheel/egg and all dependencies for a python project](https://stackoverflow.com/questions/26059111/build-a-wheel-egg-and-all-dependencies-for-a-python-project)



二、setuptools include dll

dschoni [How to get external DLLs into a python package](https://www.dschoni.de/programming/18/)



```python
from setuptools import setup, find_packages

setup(name='DLL_test',
      version='1.0.0',
      description='DLL test package',
      url='https://www.dschoni.de',
      author='Dschoni',
      author_email='scripting@dschoni.de',
      license='MIT',
      packages=find_packages(),
      package_data={'':['*.dll']}, # This is the most important line.
      zip_safe=False)
```



三、`setup.cfg` classifiers windows



## wikipedia [Setuptools](https://en.wikipedia.org/wiki/Setuptools)



## Example

imhuwq [C 拓展 Python 实战（五）—— setup.py 和 CMake](https://imhuwq.com/2020/03/22/C-%E6%8B%93%E5%B1%95-Python-%E5%AE%9E%E6%88%98%EF%BC%88%E4%BA%94%EF%BC%89%E2%80%94%E2%80%94-setup.py%20%E5%92%8C%20CMake/)

