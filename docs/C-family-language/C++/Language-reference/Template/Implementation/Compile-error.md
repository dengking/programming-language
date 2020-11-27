# Compile template error

前面描述了compiler编译template的过程，这对于找到导致compile template error的原因是有帮助的，下面是我的一些经验总结: 

draft1：如果在实例化模板的过程中，编译报错，那就是模板基类的问题了

draft2：specialization是附属，因此，如果要对template argument进行限制，则可以放到primary template中来进行实现。