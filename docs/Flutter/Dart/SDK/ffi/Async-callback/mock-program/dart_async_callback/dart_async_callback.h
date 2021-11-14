#pragma once
#include "dart_api.h"
#ifdef __cplusplus
extern "C" {
#endif
/* 下面演示的是异步回调，如果传入的cb是一个pointer to dart function，由于它会在另外一个thread中执行，那么就会导致dart VM抛出 “Cannot invoke native callback outside an isolate”
 *
 */
typedef void ( *callback_t )( void );
void __declspec( dllexport ) register_callback(callback_t cb); // 注册callback
/**
 * @brief
 *
 * @param val
*/
void __declspec( dllexport ) login(uint8_t val);


/*下面演示了使用dart concurrent library isolate的send-receive port机制来实现的*/
void __declspec( dllexport ) register_send_port(Dart_Port send_port);

void __declspec( dllexport ) login2(uint8_t val);

void __declspec( dllexport ) stop_work();

intptr_t __declspec( dllexport ) InitDartApiDL(void* data);
#ifdef __cplusplus
}
#endif