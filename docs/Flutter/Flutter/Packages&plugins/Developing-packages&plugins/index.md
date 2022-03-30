# docs [Developing packages & plugins](https://docs.flutter.dev/development/packages-and-plugins/developing-packages)

The plugin API has been updated and now supports [federated plugins](https://docs.flutter.dev/development/packages-and-plugins/developing-packages#federated-plugins) that enable separation of different platform implementations. You can also now indicate [which platforms a plugin](https://docs.flutter.dev/development/packages-and-plugins/developing-packages#plugin-platforms) supports, for example web and macOS.

> NOTE:
>
> 这是典型的跨平台技术



##  Developing plugin packages

If you want to develop a package that calls into platform-specific APIs, you need to develop a plugin package.

The API is connected to the platform-specific implementation(s) using a [platform channel](https://docs.flutter.dev/development/platform-integration/platform-channels).

### Federated plugins

> NOTE:
>
> 其实使用它的目的是separation

Federated plugins are a way of splitting support for different platforms into separate packages.

A federated plugin requires the following packages:

1、**app-facing package**

> NOTE:
>
> 这是dart 语言编写的

The package that plugin users depend on to use the plugin. This package specifies the API used by the Flutter app.

2、**platform package(s)**

> NOTE:
>
> 这是用platform-specific implementation code 编写的

One or more packages that contain the platform-specific implementation code. The app-facing package calls into these packages—they aren’t included into an app, unless they contain platform-specific functionality accessible to the end user.

3、**platform interface package**

The package that glues the **app-facing packing** to the **platform package(s)**. This package declares an interface that any platform package must implement to support the app-facing package. Having a single package that defines this interface ensures that all platform packages implement the same functionality in a uniform way.



#### Endorsed federated plugin

> NOTE:
>
> 认可的联合插件



#### Non-endorsed federated plugin

> NOTE:
>
> 未认可的联合插件



###  Specifying a plugin’s supported platforms