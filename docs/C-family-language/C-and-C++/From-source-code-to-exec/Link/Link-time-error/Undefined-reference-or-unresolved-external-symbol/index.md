# Undefined reference or unresolved external symbol



## stackoverflow [What is an undefined reference/unresolved external symbol error and how do I fix it?](https://stackoverflow.com/questions/12573816/what-is-an-undefined-reference-unresolved-external-symbol-error-and-how-do-i-fix)

> NOTE: 高赞回答



## stackoverflow [c++ undefined symbol when compiling](https://stackoverflow.com/questions/15486797/c-undefined-symbol-when-compiling)





## TODO



https://stackoverflow.com/questions/14557657/linker-error-c-undefined-reference

https://stackoverflow.com/questions/143530/linker-woes-undefined-reference

https://blog.csdn.net/huangbo1221/article/details/53438598

https://latedev.wordpress.com/2014/04/22/common-c-error-messages-2-unresolved-reference/

https://visualgdb.com/tutorials/linux/libraries/diagnosing/

https://www.linuxquestions.org/questions/programming-9/shared-library-error-undefined-reference-215785/

https://stackoverflow.com/questions/12748837/c-shared-library-undefined-reference-to-fooclasssayhello





## draft

`undefined symbol: _ZN9CppCelery11PredictTask4NAMEE	(./libfsc_quote_predict.so)`




```makefile
VPATH = .:../core:../core/celery

PREFIX := ..

INC = -I. -I.. -I../hundsun -I../third_party
SRCS := $(wildcard *.cpp)
SRCS += $(wildcard ../core/*.cpp)
SRCS += $(wildcard ../core/celery/*.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
TARGET := libfsc_quote_predict.so
LIBS = -lalgo_public -lh5api -lhiredis -lcurl

include ../make/master.mk
```





```makefile
VPATH = .:../core:../core/celery

PREFIX := ..

INC = -I. -I.. -I../hundsun -I../third_party
SRCS := $(wildcard *.cpp)
SRCS += $(wildcard ../core/*.cpp)

OBJS := $(patsubst %.cpp,%.o,$(SRCS))
TARGET := libfsc_quote_predict.so
LIBS = -lalgo_public -lh5api -lhiredis -lcurl

include ../make/master.mk
```

使用上述第二个makefile进行编译，生成的`so`就会有标题中描述的`undefined symbol`错误。显然编译过程是没有错误的。通过查看代码，我猜测应该是文件`../core/celery/tasks.cpp`没有被编译到`so`中，从而导致了上述`undefined symbol`的错误。