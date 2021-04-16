# Configuration options

一、本质是一些macro，通过这些macro来进行conditional compile，实现让programmer对library的compile过程 进行控制，一般，它们都位于`config.h`文件中

二、这是一种established idiom，下面是一些例子:

1、[Boost.Config](https://www.boost.org/doc/libs/1_75_0/libs/config/doc/html/index.html)

2、[gsl-lite](https://github.com/gsl-lite/gsl-lite)

3、[microsoft](https://github.com/microsoft)/[GSL](https://github.com/microsoft/GSL)

4、[**spdlog**](https://github.com/gabime/spdlog) `tweakme.h`