import '/D:/GitHub/programming-language/docs/Dart/Async-callback/mock-program2/async_callback_dart/dart-wrapper/dart_ffigen_wrapper/generated_bindings.dart'
    as ffigen_binding;
import 'dart:isolate';

import 'dart:ffi';
import 'package:ffi/ffi.dart';

//
final c_lib = ffigen_binding.DartAsyncCallback(DynamicLibrary.open(
    'D:\\GitHub\\programming-language\\docs\\Dart\\Async-callback\\mock-program2\\async_callback-C\\x64\\Debug\\async_callback-C.dll'));

void main(List<String> arguments) {
  c_lib.InitDartApiDL(NativeApi.initializeApiDLData);
  final interactiveCppRequests = ReceivePort()..listen(requestExecuteCallback);
  final int nativePort = interactiveCppRequests.sendPort.nativePort;
  c_lib.register_send_port(nativePort);
  int user_data = 1;
  final user_data_pointer = Pointer<Void>.fromAddress(user_data);
  final String myString = 'dk';
  Pointer<Int8> user_name = myString.toNativeUtf8().cast();
  // int id = 1;
  // Pointer<Void> user_data = malloc();
  // c_lib.login(user_name, id);
}

void requestExecuteCallback(dynamic message) {
  final int work_address = message;
  final work = Pointer<Void>.fromAddress(work_address);
  print("Dart:   Calling into C to execute callback ($work).");
  c_lib.execute_dart_callback(work);
  print("Dart:   Done with callback.");
}

Future asyncSleep(int ms) {
  return new Future.delayed(Duration(milliseconds: ms));
}
