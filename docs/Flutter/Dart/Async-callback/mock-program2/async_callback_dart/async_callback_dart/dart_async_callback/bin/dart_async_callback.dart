import '/D:/GitHub/programming-language/docs/Dart/Async-callback/mock-program2/async_callback_dart/dart-wrapper/dart_ffigen_wrapper/generated_bindings.dart'
    as ffigen_binding;
import 'dart:isolate';

import 'dart:ffi';
import 'package:ffi/ffi.dart';

var g_count = 0;
//
final c_lib = ffigen_binding.DartAsyncCallback(DynamicLibrary.open(
    'D:\\GitHub\\programming-language\\docs\\Dart\\Async-callback\\mock-program2\\async_callback-C\\x64\\Debug\\async_callback-C.dll'));

void dart_call_back(Pointer<Int8> user_name, int id, Pointer<Void> user_data) {
  print('开始执行dart callback:');
  Pointer<Utf8> user_name_str = user_name.cast();
  print(user_name_str.toDartString());
  print(id);
  // Pointer<Int64> user_data_number = user_data.cast();
  // int v = user_data_number.elementAt(0).value;
  // print(v);
  g_count++;
}

void dart_call_back2(int retcode, Pointer<Int8> str, Pointer<Void> user_data) {
  print('开始执行dart callback:');
  Pointer<Utf8> str_str = str.cast();
  print(str_str.toDartString());
  print(retcode);
  // Pointer<Int64> user_data_number = user_data.cast();
  // int v = user_data_number.elementAt(0).value;
  // print(v);
  g_count++;
}

void main(List<String> arguments) async {
  c_lib.InitDartApiDL(NativeApi.initializeApiDLData);
  final interactiveCppRequests = ReceivePort()..listen(requestExecuteCallback);
  final int nativePort = interactiveCppRequests.sendPort.nativePort;
  c_lib.register_send_port(nativePort);
  int user_data = 1;
  final user_data_pointer = Pointer<Void>.fromAddress(user_data);
  Pointer<Int8> user_name = 'dk'.toNativeUtf8().cast();
  int id = 1;
  c_lib.login(user_name, id, user_data_pointer,
      Pointer.fromFunction<ffigen_binding.login_callback_t>(dart_call_back));

  await asyncSleep(5000);

  c_lib.nim_session_cancel_stick_top(
      Pointer.fromFunction<ffigen_binding.nim_session_cancel_stick_top_cb_func>(
          dart_call_back2),
      user_data_pointer);
  while (g_count < 2) {
    print('继续等待');
    await asyncSleep(500);
  }
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
