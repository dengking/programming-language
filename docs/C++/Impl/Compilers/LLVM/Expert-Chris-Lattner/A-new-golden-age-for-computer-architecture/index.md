# 编译器黄金时代



## zhihu [LLVM创始人Chris Lattner回顾展望编译器](https://zhuanlan.zhihu.com/p/502828729)

摩尔定律失效论的讨论与日俱增，2018年，图灵奖获得者 John Hennessey 和 David Patterson 在一次演讲上更是直言，几十年来的 RISC（精简指令集）和 CISC（复杂指令集）孰优孰劣之争可以终结了，新一轮计算机架构的黄金时代已经到来，为此，在2019年的 ACM 期刊上**[发表了一篇文章里作专门论述](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzU5ODY2MTk3Nw%3D%3D%26mid%3D2247486907%26idx%3D1%26sn%3De084c7684d2f0bdbe614f49cd8114285%26chksm%3Dfe41838dc9360a9b40603233851c5050447220706da797336575a1f34b572ff04d18baf940e8%26scene%3D21%23wechat_redirect)**。

![img](https://pic4.zhimg.com/80/v2-c015ad0ef27a3bca6c7bbbe65f6c571b_1440w.webp)

为了打破当前架构发展的桎梏，给出的答案是，需要**软硬件协同**设计和创新，构建领域专用架构、领域专用语言，构建更专业化的硬件，并提升运行速度。

> NOTE:
>
> 一、"**软硬件协同**"就是 youtube [ASPLOS Keynote: The Golden Age of Compiler Design in an Era of HW/SW Co-design by Dr. Chris Lattner](https://www.youtube.com/watch?v=4HgShra-KnY) 中的"hardware and software co-design"
>
> 

weixin [LLVM之父Chris Lattner：编译器的黄金时代](https://mp.weixin.qq.com/s/dRarW4iKtjBE5Ym_P0xUQw)

youtube [ASPLOS Keynote: The Golden Age of Compiler Design in an Era of HW/SW Co-design by Dr. Chris Lattner](https://www.youtube.com/watch?v=4HgShra-KnY)



作为驱动计算机架构革新的重要组成部分，编译器也在迎来它的黄金时代。就在去年4月19日的ASPLOS会议上，编译器大牛Chris Lattner在主题演讲中分享了关于编译器的发展现状和未来、编程语言、加速器和摩尔定律失效论，并且讨论业内人士如何去协同创新，推动行业发展，实现处理器运行速度的大幅提升。OneFlow社区对其演讲内容做了不改变原意的编译，希望能对AI/编译器社区有所启发。

Chris Lattner 毕业于波特兰大学的计算机科学系，具有创建和领导多个知名大型项目的经验，其中包括 LLVM、Clang、MLIR和CIRCT等编译器基础设施项目，他还带头创建了Swift编程语言。

以下是Chris Lattner的演讲内容。



## zhihu OneFlow [LLVM之父Chris Lattner：编译器的黄金时代](https://zhuanlan.zhihu.com/p/502730940)

### 为什么需要下一代编译器和编程语言

尽管硬件正在蓬勃发展，新加速器和新技术不断涌现，但软件业却很难真正利用它们。

......

当今，**加速器**领域发生爆炸式增长，几乎每天都会有新公司制造新的加速器。但问题是，怎么用这个加速器？更关键的是，有人想做新应用，但他们想在软件代码库上下工夫，于是不停地推进和完善软件代码库。

你无法直接在这个新设备上使用旧的**软件堆栈**，它们的某个部件可能换了供应商，做了流程精简，导致所需的技术堆栈不一样。因此，你不得不给每个新的小型设备都写一个全新的**软件堆栈**。而这样做又导致了**软件的碎片化**，这种碎片化的发展带来了巨大成本，也会反噬硬件行业，因为硬件用不了了。

> NOTE:
>
> 一、"**软件的碎片化**"在 [The **LLVM** Compiler Infrastructure](https://llvm.org/)
>
> > MLIR aims to address software fragmentation, improve compilation for heterogeneous hardware, significantly reduce the cost of building domain specific compilers, and aid in connecting existing compilers together.

我的观点是，我们需要下一代编译器和编程语言来帮助解决这种碎片化。**首先，计算机行业需要更好的硬件抽象，硬件抽象是允许软件创新的方式，不需要让每种不同设备变得过于专用化。**

**其次，我们需要支持异构计算，因为要在一个混合计算矩阵里做矩阵乘法、解码JPEG、非结构化计算等等。然后，还需要适用专门领域的语言，以及普通人也可以用的编程模型。**

**最后，我们也需要具备高质量、高可靠性和高延展性的架构。**我很喜欢编译器，很多人根据编译器在做应用，我也很尊重这一点。可以说，他们在开发下一代神经网络，而不仅仅只想做编译器。大家可以合作，这样一来就意味着他们需要可用的环境和可用的工具。

令人兴奋的是，编译器或者编程语言工程师会迎来一个崭新的时代：过去和现在都有无数的技术诞生，这些技术正在改变世界，有幸参与这场变革浪潮非常令人激动。

接下来，我会谈谈编译器行业的早期发展，以及它带给我们的经验和对未来的启发。



### 传统编译器的设计和挑战

......

话说回来，LLVM虽然有很多优点，但它同样存在很多问题。一开始LLVM的目标是成为一个“万能”的解决方案，但结果它好像什么也没做好。很多人不断给LLVM加一些“乱七八糟”的东西，你可以对CPU和GPU可以这么做，但对加速器来说不太行。这种胡乱做“加法”就导致不能很多好地用LLVM做并行处理优化。但我喜欢LLVM的一点是，它是一个很好的CPU后端，尽管并不能很好满足其他需求。

现在我们来到了“摩尔定律失效期”，我们可以发扬LLVM作为CPU后端的优点，但如果要探索CPU以外的功能，应该把目光放到LLVM IR之外。

### 构建适用专用领域的架构

Patterson和Hennessey提出过一个结论：我们来到了计算机架构的文艺复兴时代，需要把计算机行业上下游人员垂直整合起来，大家既要懂硬件，也要懂软件。他们说，因为世界变化得很快，所以思考问题时要回归**第一性原理**，要用新的眼光去重新看待旧事物。

> NOTE:
>
> 一、关于 "第一性原理"，参见下面内容:
>
> 1、zhihu [什么是第一性原理？](https://zhuanlan.zhihu.com/p/41263094)
>
> 2、wikipedia [First principle](https://en.wikipedia.org/wiki/First_principle)

接下来我会讲讲加速器的构建过程，并结合经验谈谈加速器未来可能的演变。

![img](https://pic2.zhimg.com/80/v2-f1e273379c0ecd14cf397cfa45340205_1440w.webp)

如果观察硬件领域，会发现专用化架构已成为一种趋势，分化出一系列的专门品类。关于这个话题，我推荐观看Mike Urbach的演讲。如果把CPU看作通用型处理器，那么当你想控制所有的门（gate）时，就需要更深的专用化和更多硬编码能力。

所以一方面CPU是通用的，不像矩阵运算加速器那么专用化。然后出现了GPGPU，很灵活，功能也很强大，但要对GPU进行编程就没那么容易了。然后针对机器学习加速又出现了TPU，可以做大矩阵乘法运算和直接卷积等操作。这些是可编程的各种“xPU”，除此之外还有FPGA（现场可编程门阵列）等固定功能硬件，你可以重新配置block之间的线路；再进一步细分的话还有ASIC，也就是可以应特定需要专门设计集成电路。

> NOTE:
>
> 一、关于ASIC，参见baike [ASIC](https://baike.baidu.com/item/ASIC/2014676?fr=aladdin)
>
> > ASIC(Application Specific Integrated Circuit)即专用集成电路，是指应特定用户要求和特定电子系统的需要而设计、制造的[集成电路](https://baike.baidu.com/item/集成电路/108211?fromModule=lemma_inlink)。用[CPLD](https://baike.baidu.com/item/CPLD/2527470?fromModule=lemma_inlink)（[复杂可编程逻辑器件](https://baike.baidu.com/item/复杂可编程逻辑器件/12583489?fromModule=lemma_inlink)）和 [FPGA](https://baike.baidu.com/item/FPGA/935826?fromModule=lemma_inlink)（现场可编程逻辑门阵列）来进行ASIC设计是最为流行的方式之一，它们的共性是都具有用户现场可编程特性，都支持[边界扫描](https://baike.baidu.com/item/边界扫描/1150543?fromModule=lemma_inlink)技术，但两者在集成度、速度以及编程方式上具有各自的特点。

总体就这两个大类，一类是通用的、可编程的，另一类是功能比较固定的。每当我思考领域专用架构时，我的脑海里就会浮现这两大类。

![img](https://pic2.zhimg.com/80/v2-c4e0825bbdad5268ae54d9f67b27ca71_1440w.webp)

上图列举了一些正在做上述硬件的公司（不完全统计），可以看到有不少都是行业巨头。每个公司研究的时候都会思考：怎么给它编程？而每个公司也会给出不同的答案。比如Google在做XLA和TensorFlow，NVIDIA在做CUDA，Intel在做oneAPI，还有很多硬件公司在做自己的硬件设计工具包等等。


![img](https://pic3.zhimg.com/80/v2-ce9c0b60d8af2a0577564438c4f10e1e_1440w.webp)

问题是，每个工具针对的都是不同问题，它们不协同，也不兼容。因为它们是每个公司的小型团队自主研发的，共享的代码不多，而每个公司也都孜孜不倦地给自己的工具增加新功能，各个工具都瞬息万变，造成比较混乱的整体局面。这些工具作为行业的基本组件，却有这么多不同特点，那从行业层面应该怎么做？

![img](https://pic1.zhimg.com/80/v2-866b2049fd874505f42939e729e3eb88_1440w.webp)

其实今天的加速器遇到的问题，90年代的C语言编译器也遇到过。就像人们常说的那样，历史是一个轮回。我们见证了硬件和软件的多样性爆发，但如果想要继续发展，这种多样性就会成为阻碍。所以我们需要统一，需要一些类似GCC和LLVM这样的东西，不然都要忙着为每个特定的设备开发一个专属后端，就没时间进行前端、编程语言和模型的创新了。

业内有许多精英人才，但还不够多。假如我们能够减少碎片化，把行业整合起来，就可以促进创新，加快行业发展，持续建立技术堆栈，充分利用硬件，并以全新方式利用异构计算。接下来谈谈我对加速器发展的看法，以及发展过程中可能遇到的挑战。





## zhihu [现在是不是编译器行业的风口时期？](https://www.zhihu.com/question/471096068/answer/1990950901)&#x20;



## weixin [两大图灵奖得主力作：计算机架构的新黄金时代](https://mp.weixin.qq.com/s/5ruauCn1vQswyo7lIrp6BQ)

