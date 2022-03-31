# flutter windows headers

## channel、messenger、codec

```c++
  // Creates an instance that sends and receives method calls on the channel
  // named |name|, encoded with |codec| and dispatched via |messenger|.
```



## channel

| header                                                       | class                 |      |
| ------------------------------------------------------------ | --------------------- | ---- |
| [**basic_message_channel.h**](https://github.com/flutter/engine/blob/main/shell/platform/common/client_wrapper/include/flutter/basic_message_channel.h) | `BasicMessageChannel` |      |
| [**event_channel.h**](https://github.com/flutter/engine/blob/main/shell/platform/common/client_wrapper/include/flutter/event_channel.h) | `EventChannel`        |      |
| [**method_channel.h**](https://github.com/flutter/engine/blob/main/shell/platform/common/client_wrapper/include/flutter/method_channel.h) | `MethodChannel`       |      |

上述channel，有非常多的共性：

一、成员变量：

```c++
  BinaryMessenger* messenger_;
  std::string name_;
  const MessageCodec<T>* codec_;
```

二、构造函数:

```c++
  // Creates an instance that sends and receives method calls on the channel
  // named |name|, encoded with |codec| and dispatched via |messenger|.
  BasicMessageChannel(BinaryMessenger* messenger,
                      const std::string& name,
                      const MessageCodec<T>* codec)
      : messenger_(messenger), name_(name), codec_(codec) {}
```



