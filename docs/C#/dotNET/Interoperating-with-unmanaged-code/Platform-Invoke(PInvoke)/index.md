# Platform Invoke (P/Invoke)

## microsoft [Platform Invoke (P/Invoke)](https://docs.microsoft.com/en-us/dotnet/standard/native-interop/pinvoke)



### Invoking managed code from unmanaged code

The runtime allows communication to flow in both directions, enabling you to call back into **managed code** from native functions by using **function pointers**. The closest thing to a function pointer in managed code is a **delegate**, so this is what is used to allow callbacks from native code into managed code.

The way to use this feature is similar to the managed to native process previously described. For a given **callback**, you define a **delegate** that matches the signature and pass that into the **external method**. The runtime will take care of everything else.



## [**PInvoke.net**](https://www.pinvoke.net/)