#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
using Dart_Port = int64_t;
using WorkT = std::function<void()>;

struct CApiVectorOfNim_onlineclient {
    int field;
};

struct StructT {
    int field;
};
enum EnumT { field = 1 };

void CallBack(void* UserData, StructT* p1) {}
using CallBackT = void (*)(void*, StructT*);
void CallBack2(void* UserData, CApiVectorOfNim_onlineclient* p1) {}
using CallBack2T = void (*)(void*, CApiVectorOfNim_onlineclient*);
/**
 * @brief dart环境
 *
 */
class DartEnv {
    static Dart_Port dart_send_port;

public:
    /**
     * @brief 判断当前是否处于dart环境
     *
     * @return true
     * @return false
     */
    static inline bool IsInDartEnv() { return dart_send_port > 0; }

    /**
     * @brief
     *
     * @param send_port
     */
    static void RegisterSendPort(Dart_Port send_port) { dart_send_port = send_port; }
    /**
     * @brief 非阻塞式的，即不关注dart callback的返回值
     * @tparam DartCallBackT
     * @tparam ...Args
     * @param cb C callback C回调
     * @param ...args
     */
    template <typename CallBackT, typename... Args>
    static bool PostTaskNonblocking(CallBackT&& cb, Args&&... args) {
        // 全部都按值来进行捕获
        WorkT work = [=]() mutable {
            if (cb) {
                /**
                 * @brief 需要将args转化为 C 类型:
                 * 1、如果是普通类型，则需要返回指针
                 * 2、如果是shared_ptr<>，则需要get
                 * 3、
                 */
                cb(Convert2CArg(std::forward<Args>(args))...);
            }
        };
        WorkT* w = new WorkT(work);  // 在 execute_dart_callback 中，会将它析构的
        return NotifyDart(w);
    }

    /**
     * @brief Notify Dart through a port that the C lib has pending async callbacks. Expects heap allocated `work` so
     * delete can be called on it. The `send_port` should be from the isolate which registered the callback.
     *
     * @param work
     * @return true 发送成功
     * @return false 发送失败
     */
    static bool NotifyDart(const WorkT* work) {
        const intptr_t work_addr = reinterpret_cast<intptr_t>(work);

        // Dart_CObject dart_object;
        // dart_object.type = Dart_CObject_kInt64;
        // dart_object.value.as_int64 = work_addr;

        // return Dart_PostCObject_DL(dart_send_port, &dart_object);
        return true;
    }
    /**
     * @brief 将由dart来调用这个函数
     * @param work_ptr
     */
    static void ExecuteDartCallback(void* work_ptr) {
        WorkT* work = reinterpret_cast<WorkT*>(work_ptr);
        (*work)();    // 执行
        delete work;  // 析构
    }

private:
    /**
     * @brief 如果是struct、class，则需要pass by pointer
     *
     * @tparam T
     * @param t
     * @return std::enable_if<std::is_class<T>::value, T*>::type
     */
    template <typename T>
    static auto Convert2CArg(T&& t) ->
        typename std::enable_if<std::is_class<typename std::remove_reference<T>::type>::value,
                                typename std::remove_reference<T>::type*>::type {
        return &t;
    }
    /**
     * @brief 如果是算术类型、enum，则需要pass by value
     *
     * @tparam T
     * @param t
     * @return std::enable_if<std::is_class<T>::value, T>::type
     */
    template <typename T>
    static auto Convert2CArg(T&& t) ->
        typename std::enable_if<std::is_arithmetic<typename std::remove_reference<T>::type>::value ||
                                    std::is_enum<typename std::remove_reference<T>::type>::value,
                                typename std::remove_reference<T>::type>::type {
        return t;
    }
    /**
     * @brief
     *
     * @param s
     * @return const char*
     */
    static const char* Convert2CArg(std::shared_ptr<std::string> s) { return s.get()->c_str(); }
    /**
     * @brief
     *
     * @tparam T
     * @param t
     * @return std::enable_if<std::is_pointer<T>::value, T>::type
     */
    template <typename T>
    static auto Convert2CArg(T&& t) ->
        typename std::enable_if<std::is_pointer<typename std::remove_reference<T>::type>::value,
                                typename std::remove_reference<T>::type>::type {
        return t;
    }
    /**
     * @brief 如果是shared_ptr to C vector type，则需要拿到它的pointee
     *
     * @tparam T
     * @param s
     * @return const char*
     */
    static const CApiVectorOfNim_onlineclient* Convert2CArg(std::shared_ptr<CApiVectorOfNim_onlineclient> p) {
        return p.get();
    }
};
int main() {
    StructT struct1;
    EnumT enum1 = EnumT::field;
    CallBackT c1 = &CallBack;
    void* userdata = &struct1;
    DartEnv::PostTaskNonblocking(c1, userdata, &struct1);
}
