# diary yunxin rtc windows plugin

按照在docs [Developing packages & plugins](https://docs.flutter.dev/development/packages-and-plugins/developing-packages) 中的介绍，执行如下命令：

```shell
PS D:\NetEase\yunxin-sdk-flutter\packages\nertc_core> flutter create --template=plugin --platforms=windows . --org com.netease.nertcflutter
Flutter assets will be downloaded from https://storage.flutter-io.cn. Make sure you trust this source!
Recreating project ....
  .idea\runConfigurations\example_lib_main_dart.xml (created)
  .metadata (created)
  analysis_options.yaml (created)
  android\nertc_core_android.iml (created)
  android\src\main\kotlin\com\netease\nertcflutter\nertc_core\NertcCorePlugin.kt (created)
  ios\Classes\NertcCorePlugin.h (created)
  ios\Classes\NertcCorePlugin.m (created)
  ios\Classes\SwiftNertcCorePlugin.swift (created)
  ios\nertc_core.podspec (created)
  lib\nertc_core.dart (created)
  test\nertc_core_test.dart (created)
  windows\CMakeLists.txt (created)
  windows\include\nertc_core\nertc_core_plugin.h (created)
  windows\nertc_core_plugin.cpp (created)
Running "flutter pub get" in nertc_core...                         508ms
  example\.idea\libraries\Dart_SDK.xml (created)
  example\.idea\libraries\KotlinJavaRuntime.xml (created)
  example\.idea\modules.xml (created)
  example\.idea\runConfigurations\main_dart.xml (created)
  example\.idea\workspace.xml (created)
  example\analysis_options.yaml (created)
  example\android\app\src\main\kotlin\com\netease\nertcflutter\nertc_core_example\MainActivity.kt (created)
  example\android\nertc_core_example_android.iml (created)
  example\android\app\src\main\res\drawable-v21\launch_background.xml (created)
  example\android\app\src\main\res\values-night\styles.xml (created)
  example\ios\Runner\AppDelegate.swift (created)       
  example\ios\Runner\Runner-Bridging-Header.h (created)
  example\ios\Runner.xcodeproj\project.xcworkspace\xcshareddata\IDEWorkspaceChecks.plist (created)
  example\ios\Runner.xcworkspace\xcshareddata\IDEWorkspaceChecks.plist (created)
  example\ios\Runner.xcworkspace\xcshareddata\WorkspaceSettings.xcsettings (created)
  example\nertc_core_example.iml (created)
  example\windows\.gitignore (created)
  example\windows\CMakeLists.txt (created)
  example\windows\flutter\CMakeLists.txt (created)
  example\windows\runner\CMakeLists.txt (created)
  example\windows\runner\flutter_window.cpp (created)
  example\windows\runner\flutter_window.h (created)
  example\windows\runner\main.cpp (created)
  example\windows\runner\resource.h (created)
  example\windows\runner\resources\app_icon.ico (created)
  example\windows\runner\runner.exe.manifest (created)
  example\windows\runner\Runner.rc (created)
  example\windows\runner\utils.cpp (created)
  example\windows\runner\utils.h (created)
  example\windows\runner\win32_window.cpp (created)
  example\windows\runner\win32_window.h (created)
Running "flutter pub get" in example...                          1,950ms
Wrote 48 files.

All done!

Your plugin code is in .\lib\nertc_core.dart.

Your example app code is in .\example\lib\main.dart.


Host platform code is in the windows directories under ..
To edit platform code in an IDE see https://flutter.dev/developing-packages/#edit-plugin-package.


You need to update ./pubspec.yaml to support windows.



To add platforms, run `flutter create -t plugin --platforms <platforms> .` under ..
For more information, see https://flutter.dev/go/plugin-platforms.

```



输出中，下面内容被标红了：

```
You need to update ./pubspec.yaml to support windows.
```

它是为了提示工程师，需要在 `./pubspec.yaml` 中添加Windows channel的支持，我参考 [Agora-Flutter-SDK](https://github.com/AgoraIO/Agora-Flutter-SDK)/[**pubspec.yaml**](https://github.com/AgoraIO/Agora-Flutter-SDK/blob/master/pubspec.yaml) 中的写法，做了如下修改：

![](./pubspec-change.png)



