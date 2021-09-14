#include "dart_async_callback.h"
#include "dart_native_api.h"
#include "dart_api_dl.h"

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
//#include "pch.h"

callback_t g_cb; // 回调函数
uint8_t g_val; // 回调函数的入参

std::mutex m1;
std::condition_variable cv1;
bool ready1 = false;

/**
* @brief 异步执行回调
*
*/
std::thread* t1;


/**
 * @brief 设置回调函数
 * @param cb
*/
void register_callback(callback_t cb)
{
	g_cb = cb;


}

/**
 * @brief 设置val的值，唤醒线程执行回调函数
 * @param val
*/
void login(uint8_t val)
{
	{
		std::lock_guard<std::mutex> lk(m1);
		ready1 = true;
		g_val = val;
	}
	t1 = new std::thread([] {
		std::this_thread::sleep_for(std::chrono::seconds(3));
		( *g_cb )( ); // 执行回调函数
		});
}

////////////////////////////////////////////////////////////////////////////

std::mutex m2;
std::condition_variable cv2;
bool ready2 = false;
uint8_t g_val2;

Dart_Port g_send_port;

static void FreeFinalizer(void*, void* value) {
	free(value);
}

void internal_callback(uint8_t i)
{
	std::cout << "begin send message" << std::endl;
	const char* methodname = "myCallback1";
	void* request_buffer = malloc(sizeof(uint8_t) * 1);  // FreeFinalizer.
	reinterpret_cast<uint8_t*>( request_buffer )[0] = i;   // Populate buffer.
	const size_t request_length = sizeof(uint8_t) * 1;

	Dart_CObject c_send_port;
	c_send_port.type = Dart_CObject_kNull;

	Dart_CObject c_pending_call;
	c_pending_call.type = Dart_CObject_kNull;

	Dart_CObject c_method_name;
	c_method_name.type = Dart_CObject_kString;
	c_method_name.value.as_string = const_cast<char*>( methodname );

	Dart_CObject c_request_data;
	c_request_data.type = Dart_CObject_kExternalTypedData;
	c_request_data.value.as_external_typed_data.type = Dart_TypedData_kUint8;
	c_request_data.value.as_external_typed_data.length = request_length;
	c_request_data.value.as_external_typed_data.data =
		static_cast<uint8_t*>( request_buffer );
	c_request_data.value.as_external_typed_data.peer = request_buffer;
	c_request_data.value.as_external_typed_data.callback = FreeFinalizer;

	Dart_CObject* c_request_arr[] = { &c_send_port, &c_pending_call,
									 &c_method_name, &c_request_data };
	Dart_CObject c_request;
	c_request.type = Dart_CObject_kArray;
	c_request.value.as_array.values = c_request_arr;
	c_request.value.as_array.length =
		sizeof(c_request_arr) / sizeof(c_request_arr[0]);

	/*printf("C   :  Dart_PostCObject_(request: %" Px ", call: %" Px ").\n",
		reinterpret_cast<intptr_t>( &c_request ),
		reinterpret_cast<intptr_t>( &c_pending_call ));*/
	std::cout << "Dart_PostCObject_DL" << std::endl;
	Dart_PostCObject_DL(g_send_port, &c_request);
	std::cout << "finish send message" << std::endl;
}

/**
 * @brief 异步执行回调
 *
*/
std::thread* t2;



/**
 * @brief 设置发送port
 * @param send_port
*/
void register_send_port(Dart_Port send_port)
{
	g_send_port = send_port;
	
}


/**
 * @brief
 *
 * @param val
*/
void login2(uint8_t val)
{
	g_val2 = val;
	t2 = new std::thread([] {
		std::this_thread::sleep_for(std::chrono::microseconds(200));
		internal_callback(g_val2);

		std::this_thread::sleep_for(std::chrono::microseconds(2000));
		});
}

void stop_work()
{
	t2->join();
}

intptr_t InitDartApiDL(void* data) {
	intptr_t res = Dart_InitializeApiDL(data);
	std::cout << res << std::endl;
	return res;
}




