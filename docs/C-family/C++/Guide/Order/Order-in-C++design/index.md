# Inter-thread 和 intra-thread

## intra-thread

1、Sequenced-before

2、Carries dependency



## inter-thread

1、Inter-thread happens-before



## memory ordering总结

### Modification order

1、Modification order是一个全局的order

2、Modification order是基于 "happens-before" 而建立的



### `std::memory_order`

1、它是控制single thread的memory order

2、memory ordering 所有的thread 按照系统的order去access shared atomic variable，而不是去contention

a、对一个shared data的read、write order进行控制，从而避免data race，达到目的

b、这应该是preshing cooperative的含义吧