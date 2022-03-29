# [Writing custom platform-specific code](https://docs.flutter.dev/development/platform-integration/platform-channels)

> NOTE:
>
> 1、method channel
>
> message passing
>
> 2、serialization and deserialization
>
> 3、UI  client、platform host

## Architectural overview: platform channels

Messages are passed between the client (UI) and host (platform) using platform channels as illustrated in this diagram:

![Platform channels architecture](https://docs.flutter.dev/assets/images/docs/PlatformChannels.png)

Messages and responses are passed asynchronously, to ensure the user interface remains responsive.

On the client side, [`MethodChannel`](https://api.flutter.dev/flutter/services/MethodChannel-class.html) enables sending messages that correspond to method calls. On the platform side, `MethodChannel` on Android ([`MethodChannelAndroid`](https://api.flutter.dev/javadoc/io/flutter/plugin/common/MethodChannel.html)) and `FlutterMethodChannel` on iOS ([`MethodChanneliOS`](https://api.flutter.dev/objcdoc/Classes/FlutterMethodChannel.html)) enable receiving method calls and sending back a result. These classes allow you to develop a platform plugin with very little ‘boilerplate’ code.





## [flutter](https://github.com/flutter/flutter)/[examples](https://github.com/flutter/flutter/tree/master/examples)/[platform_channel](https://github.com/flutter/flutter/tree/master/examples/platform_channel)/[lib](https://github.com/flutter/flutter/tree/master/examples/platform_channel/lib)/[**main.dart**](https://github.com/flutter/flutter/blob/master/examples/platform_channel/lib/main.dart)

> NOTE:
>
> `methodChannel` 的入参数是什么？

