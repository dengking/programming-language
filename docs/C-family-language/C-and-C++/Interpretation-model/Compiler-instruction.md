# interpretion

compiler根据type来汇编生成instruction，在硬件级别，没有type信息，在硬件级别，只有01。指令对01数据进行操作，比如加法指令，浮点运算指令。 在语言基本是function and data，在硬件级别是instruction and data

显然，对于浮点类型，compiler会选择浮点运算指令，对于整形，则会选择简单指令。

上述分析非常重要 它是理解很多C，C++中pointer conversion的基础。

将此作成为interpretation model。

这个model非常重要，它可以描述：

\- object representation

\- union

\- aliasing