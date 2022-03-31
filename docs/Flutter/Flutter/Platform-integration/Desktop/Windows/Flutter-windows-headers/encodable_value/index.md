# [encodable_value.h](https://engine.chinmaygarde.com/encodable__value_8h_source.html)

## `class CustomEncodableValue`

基本上等同于`std::any`。

## `class EncodableValue`



```c++
using EncodableList = std::vector<EncodableValue>;
using EncodableMap = std::map<EncodableValue, EncodableValue>;
```



```c++
// As an example, the following Dart structure:
  {
    'flag': true,
    'name': 'Thing',
    'values': [1, 2.0, 4],
  }
// would correspond to:
  EncodableValue(EncodableMap{
      {EncodableValue("flag"), EncodableValue(true)},
      {EncodableValue("name"), EncodableValue("Thing")},
      {EncodableValue("values"), EncodableValue(EncodableList{
                                     EncodableValue(1),
                                     EncodableValue(2.0),
                                     EncodableValue(4),
                                 })},
  })
```

