#include "async_callback-C.h"
#include <functional>
#include <thread>
#include <utility>
#include <string>
#include <memory>

#include "dart_native_api.h"
#include "dart_api_dl.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

void Fatal(char const* file, int line, char const* error) {
	printf("FATAL %s:%i\n", file, line);
	printf("%s\n", error);
	abort();
}

#define FATAL(error) Fatal(__FILE__, __LINE__, error)

Dart_Port g_send_port;

using WorkT = std::function<void()>;

// Notify Dart through a port that the C lib has pending async callbacks.
//
// Expects heap allocated `work` so delete can be called on it.
//
// The `send_port` should be from the isolate which registered the callback.
void NotifyDart(Dart_Port send_port, const WorkT* work) {
	const intptr_t work_addr = reinterpret_cast<intptr_t>( work );

	Dart_CObject dart_object;
	dart_object.type = Dart_CObject_kInt64;
	dart_object.value.as_int64 = work_addr;

	const bool result = Dart_PostCObject_DL(send_port, &dart_object);
	if (!result) {
		FATAL("C   :  Posting message to port failed.");
	}
}
/**
 * @brief 非阻塞式的，即不关注dart callback的返回值
 * @tparam DartCallBackT
 * @tparam ...Args
 * @param dart_cb
 * @param ...args
*/
template<typename DartCallBackT, typename... Args>
void internal_callback_non_blocking(DartCallBackT&& dart_cb, Args&&...args)
{
	// 全部都按值来进行捕获
	WorkT work = [=]() {
		if (dart_cb)
			( *dart_cb )( std::forward<Args&&>(args)... );
	};
	WorkT* w = new WorkT(work); // 在 execute_dart_callback 中，会将它析构的
	NotifyDart(g_send_port, w);
}

/**
 * @brief 非阻塞式的，即不关注dart callback的返回值
 * @tparam DartCallBackT
 * @tparam ...Args
 * @param dart_cb
 * @param ...args
*/
template<typename DartCallBackT, typename... Args>
void internal_callback_non_blocking(DartCallBackT&& dart_cb, int rescode, std::shared_ptr<std::string> str, Args&&...args)
{
	const char* c_str = str->c_str();
	internal_callback_non_blocking(dart_cb, rescode, c_str, std::forward<Args&&>(args)...);
}

/**
 * @brief 非阻塞式的，即不关注dart callback的返回值
 * @tparam DartCallBackT
 * @tparam ...Args
 * @param dart_cb
 * @param ...args
*/
template<typename DartCallBackT, typename... Args>
void internal_callback_non_blocking(DartCallBackT&& dart_cb, std::shared_ptr<std::string> str, Args&&...args)
{
	internal_callback_non_blocking(dart_cb, str->c_str(), std::forward<Args&&>(args)...);
}

std::thread* t; // 执行异步回调
std::thread* t2; // 执行异步回调
void login(const char* user_name, int64_t id, void* user_data, login_callback_t cb)
{
	// 模拟异步回调
	t = new std::thread([=] {
		std::this_thread::sleep_for(std::chrono::microseconds(300));
		internal_callback_non_blocking(cb, user_name, id, user_data);
		}
	);
}

void stop()
{
	t->join();
	t2->join();
}
/**
 * @brief 将由dart来调用这个函数
 * @param work_ptr
*/
void execute_dart_callback(void* work_ptr) {
	( *reinterpret_cast<WorkT*>( work_ptr ) )( ); // 执行
	delete work_ptr; // 
	printf("C:    ExecuteCallback done.\n");
}

intptr_t InitDartApiDL(void* data)
{
	return Dart_InitializeApiDL(data);
}

/**
 * @brief 设置发送port
 * @param send_port
*/
void register_send_port(Dart_Port send_port)
{
	g_send_port = send_port;

}


void nim_session_cancel_stick_top(nim_session_cancel_stick_top_cb_func cb, const void* user_data)
{
	int rescode = 0;
	std::shared_ptr<std::string> session_id{ new std::string("hello world") };
	// 模拟异步回调
	t = new std::thread([=] {
		std::this_thread::sleep_for(std::chrono::microseconds(300));
		// 全部都按值来进行捕获
		WorkT work = [=]() {
			if (cb)
				( *cb )( rescode, session_id->c_str(), user_data );
		};
		WorkT* w = new WorkT(work); // 在 execute_dart_callback 中，会将它析构的
		NotifyDart(g_send_port, w);
		}
	);
}


NIM_SDK_DLL_API void nim_session_cancel_stick_top(const char* session_id, enum NIMSessionType to_type, nim_session_cancel_stick_top_cb_func cb, const void* user_data) {
	QLOG_APP("[api]session.call cancel stick top");
	RemoveStickTopSessionParam param;
	param.session_id = session_id;
	param.to_type = to_type;
	nim::SessionManager::GetInstance()->InvokeRemoveStickTopSession(param, [cb, user_data](int rescode, std::shared_ptr<std::string> session_id, int to_type) {
		internal_callback_non_blocking(cb, rescode, session_id, user_data);
}