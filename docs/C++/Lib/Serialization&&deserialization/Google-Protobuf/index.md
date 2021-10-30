# Protocol Buffers



## developers.google [Protocol Buffers](https://developers.google.com/protocol-buffers)



## Auxiliary compiler

"tag-auxiliary compiler DSL code generator stub code(桩代码)-Google-protobuf-IDL"

## Binary serialization of struct vs protoc-buff

在ust项目中，直接使用的binary serialization，为每种请求都涉及一个`struct`，然后client和server之间就使用`struct`来作为protocol。显然这种实现方式是最最高效的，但是这种实现方式所带来的一个问题是：每次新增一个请求，就涉及到client和server的全部的修改。而不是像普通的协议那样。与此相关的是redis的协议、http协议。

https://stackoverflow.com/questions/2966500/protobuf-net-not-faster-than-binary-serialization

https://theburningmonk.com/2011/08/performance-test-binaryformatter-vs-protobuf-net/

