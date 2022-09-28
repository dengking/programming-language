# Python packaging		

## [Distributing Python Modules](https://docs.python.org/3/distributing/index.html#distributing-python-modules)

### Key terms

[`distutils`](https://docs.python.org/3/library/distutils.html#module-distutils) 

> NOTE: 
>
> 已经被淘汰

[setuptools](https://setuptools.readthedocs.io/en/latest/) is a (largely) drop-in replacement for [`distutils`](https://docs.python.org/3/library/distutils.html#module-distutils) first published in 2004.

> NOTE: 
>
> 当前的主流工具，支持声明dependency

[wheel](https://wheel.readthedocs.io/) 

> NOTE: 
>
> 一、一种包格式
>
> 二、[PEP 427 – The Wheel Binary Package Format 1.0](https://peps.python.org/pep-0427/)
>
> 三、implementation: [wheel](https://wheel.readthedocs.io/en/stable/)
>
> 

[wheel](https://wheel.readthedocs.io/) (in this context) is a project that adds the `bdist_wheel` command to [`distutils`](https://docs.python.org/3/library/distutils.html#module-distutils)/[setuptools](https://setuptools.readthedocs.io/en/latest/). This produces a cross platform binary packaging format (called “wheels” or “wheel files” and defined in [**PEP 427**](https://www.python.org/dev/peps/pep-0427)) that allows Python libraries, even those including binary extensions, to be installed on a system without needing to be built locally.

 

### Installing the tools



```shell
python -m pip install setuptools wheel twine
```

## [Python Packaging Authority](https://www.pypa.io/en/latest/)

> NOTE:
>
> 一、这是专门研发Python packaging的

The software developed through the PyPA is used to package, share, and install Python software and to interact with indexes of downloadable Python software such as [PyPI](https://pypi.org/), the Python Package Index.

> NOTE:
>
> 一、PyPA 和 PyPI 之间的对应关系



### [Tutorials](https://packaging.python.org/en/latest/tutorials/)

1、[Installing Packages](https://packaging.python.org/en/latest/tutorials/installing-packages/)

> NOTE:
>
> 一、主要介绍pip、[Creating Virtual Environments](https://packaging.python.org/en/latest/tutorials/installing-packages/#id16) 等内容

2、[Managing Application Dependencies](https://packaging.python.org/en/latest/tutorials/managing-dependencies/)

> NOTE:
>
> 一、下面单独介绍了

3、[Packaging Python Projects](https://packaging.python.org/en/latest/tutorials/packaging-projects/)

4、[Creating Documentation](https://packaging.python.org/en/latest/tutorials/creating-documentation/)



### [Guides](https://packaging.python.org/en/latest/guides/)



## docs [Packaging Python Projects](https://packaging.python.org/en/latest/tutorials/packaging-projects/#packaging-python-projects)

This tutorial walks you through how to package a simple Python project. It will show you how to add the necessary files and structure to create the package, how to build the package, and how to upload it to the Python Package Index (PyPI).

### Creating the package files

> NOTE:
>
> 一、
>
> | package file     |                                                              |      |
> | ---------------- | ------------------------------------------------------------ | ---- |
> | `pyproject.toml` | `pyproject.toml` tells “frontend” build tools like [pip](https://packaging.python.org/en/latest/key_projects/#pip) and [build](https://packaging.python.org/en/latest/key_projects/#build) what “backend” tool to use to create [distribution packages](https://packaging.python.org/en/latest/glossary/#term-Distribution-Package) for your project. |      |
> |                  |                                                              |      |
> |                  |                                                              |      |
>
> 

#### Creating `pyproject.toml`

> NOTE: 
>
> 一、这是一种比较新的方式
>
> 二、`pyproject.toml` VS `setup.cfg`
>
> 1、stackoverflow [Is `setup.cfg` deprecated?](https://stackoverflow.com/questions/44878600/is-setup-cfg-deprecated)
>
> The plan is to deprecate `setup.cfg` eventually - follow [*#3214 Eventually deprecate setup.cfg with automatic conversion to pyproject.toml*](https://github.com/pypa/setuptools/issues/3214) for progress. 
>
> – [wim](https://stackoverflow.com/users/674039/wim) [Apr 21 at 1:28](https://stackoverflow.com/questions/44878600/is-setup-cfg-deprecated#comment127135225_44878600) 
>
> 



