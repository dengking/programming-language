import '/D:/GitHub/programming-language/docs/Dart/Async-callback/mock-program/dart-project/dart_async_callback2/dart_async_callback2/generated_bindings.dart'
    as binding;
import 'dart:ffi';

import 'dart:isolate';
import 'dart:typed_data';

//
final dyLib = DynamicLibrary.open(
    'D:\\GitHub\\programming-language\\docs\\Dart\\Async-callback\\mock-program\\dart_async_callback\\x64\\Debug\\dart_async_callback.dll');
final c_lib = binding.DartAsyncCallback(dyLib);

Future asyncSleep(int ms) {
  return new Future.delayed(Duration(milliseconds: ms), () => true);
}

void callBack() {
  print('callback');
}

void bad() {
  c_lib.register_callback(Pointer.fromFunction<binding.callback_t>(callBack));
  c_lib.login(1);
}

class CppRequest {
  final SendPort? replyPort;
  final int? pendingCall;
  final String method;
  final Uint8List data;

  factory CppRequest.fromCppMessage(List message) {
    return CppRequest._(message[0], message[1], message[2], message[3]);
  }

  CppRequest._(this.replyPort, this.pendingCall, this.method, this.data);

  String toString() => 'CppRequest(method: $method, ${data.length} bytes)';
}

int g_count = 0;
void handleCppRequests(dynamic message) {
  final cppRequest = CppRequest.fromCppMessage(message);
  print('Dart:   Got message: $cppRequest');

  if (cppRequest.method == 'myCallback1') {
    // Use the data in any way you like. Here we just take the first byte as
    // the argument to the function.
    g_count = cppRequest.data[0];
    print(g_count);
  } else {}
}

void good() async {}

void main(List<String> arguments) async {
  c_lib.InitDartApiDL(NativeApi.initializeApiDLData);
  final interactiveCppRequests = ReceivePort()..listen(handleCppRequests);
  final int nativePort = interactiveCppRequests.sendPort.nativePort;
  c_lib.register_send_port(nativePort);
  c_lib.login2(1);
  while (g_count <= 0) {
    print('继续等待');
    await asyncSleep(500);
  }
  print("Dart: Tell C to stop worker threads.");
  c_lib.stop_work();
  interactiveCppRequests.close();
  print(g_count);
  print('结束');
}
