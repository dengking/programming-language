texture

wikipedia [Texture mapping](https://en.wikipedia.org/wiki/Texture_mapping)



"Pixel buffer-based texture"

"pixel buffer texture"

"image buffer object"



"render thread"



"texture frame"

"texture ID"



"surface": width、height



## github [flutter_texture_registrar.h](https://github.com/flutter/engine/blob/main/shell/platform/common/public/flutter_texture_registrar.h)

从中的内容可知，目前flutter desktop只支持"pixel buffer-based texture"。

pixel buffer的定义：

`FlutterDesktopPixelBuffer`

pixel buffer-based texture的C定义：

```c++
FlutterDesktopTextureInfo
```



### `FlutterDesktopTextureRegistrar`



### register、unregister texture

|                                                              |      |      |
| ------------------------------------------------------------ | ---- | ---- |
| `FlutterDesktopTextureRegistrarRegisterExternalTexture`      |      |      |
| `FlutterDesktopTextureRegistrarUnregisterExternalTexture`    |      |      |
| `FlutterDesktopTextureRegistrarMarkExternalTextureFrameAvailable` |      |      |



## github [**texture_registrar.h**](https://github.com/flutter/engine/blob/main/shell/platform/common/client_wrapper/include/flutter/texture_registrar.h)

pixel buffer-based texture的C++定义：

```C++
class PixelBufferTexture 
```

