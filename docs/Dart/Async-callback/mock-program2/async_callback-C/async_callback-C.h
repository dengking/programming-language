#pragma once

#include "dart_api.h"

#if !defined(__GNUC__) && defined(NIM_SDK_EXPORTS)
#define NIM_SDK_DLL_API __declspec(dllexport)
#elif !defined(__GNUC__) && !defined(NIM_SDK_EXPORTS)
#define NIM_SDK_DLL_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(NIM_SDK_EXPORTS)
#define NIM_SDK_DLL_API __attribute__((visibility("default")))
#else
#define NIM_SDK_DLL_API
#endif


#ifdef __cplusplus
extern "C" {
#endif
	/**
	 * @brief
	*/
	NIM_SDK_DLL_API void register_send_port(Dart_Port send_port);

	typedef void( *login_callback_t )( const char* user_name, int64_t id, void* user_data );
	/**
	 * @brief
	 * @param cb
	*/
	NIM_SDK_DLL_API void login(const char* user_name, int64_t id, void* user_data, login_callback_t cb);

	NIM_SDK_DLL_API void stop();
	
	NIM_SDK_DLL_API void execute_dart_callback(void* work);

	NIM_SDK_DLL_API intptr_t InitDartApiDL(void* data);

	typedef void ( *nim_session_cancel_stick_top_cb_func )( int rescode, const char* session_id, const void* user_data );

	NIM_SDK_DLL_API void nim_session_cancel_stick_top(nim_session_cancel_stick_top_cb_func cb, const void* user_data);

#ifdef __cplusplus
}
#endif