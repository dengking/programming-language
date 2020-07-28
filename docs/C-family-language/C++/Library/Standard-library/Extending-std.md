# Extending std



## cppreference [Extending the namespace `std`](https://en.cppreference.com/w/cpp/language/extending_std)

### Adding declarations to `std`

It is **undefined behavior** to add declarations or definitions to `namespace std` or to any namespace nested within `std`, with a few exceptions noted below

> NOTE: 这说明我们不能直接向`std`中添加declaration。

### Adding template specializations

#### Class templates



#### Function templates and member functions of templates



#### Variable templates