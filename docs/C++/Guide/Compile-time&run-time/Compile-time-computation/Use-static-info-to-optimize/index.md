# Use static info to optimization

> 在 relocate paper 中，对此进行了非常好的说明，我是在阅读它的时候，想到的；其中谈及了:
>
> static-reflection+conditional compile+static polymorphism
>
> zero overhead-optimization
>
> 

一、

C++支持非常强大的static reflection、static polymorphism，这就给予了C++非常强大的:

1、use static info to optimize

2、custom static polymorphism to select best at compile time

polymorphism 的本质目的是选择 "best"，C++的static polymorphism让programmer能够在compile time完成select best。

二、遵循 zero overhead principle

static reflection + optimization

三、这是遵循 [CppCoreGuidelines Per.11: Move computation from run time to compile time](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rper-Comp)

简而言之: move runtime to compile time

```c++
double square(double d) { return d * d; }

static double s2 = square(2);    // old-style: dynamic initialization

// assume 0 <= n
constexpr double ntimes(double d, int n) {
    double m = 1;
    while (n--) m *= d;
    return m;
}

constexpr double s3{ntimes(2, 3)};  // modern-style: compile-time initialization

```



## Example

1、relocate paper: 如果知道是trivially-relocatable那么可以直接使用realloc

2、如果知道是trivial，那么就可以像C object一样直接使用memory operation进行操作，这就省去了constructor、destructor

3、cppcoreguidelines move runtime to compile time的small buffer optimization的例子

4、cppcoreguidelines [Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) # Notes

To make the [strong exception guarantee](https://en.cppreference.com/w/cpp/language/exceptions#Exception_safety) possible, user-defined move constructors should not throw exceptions. For example, [std::vector](https://en.cppreference.com/w/cpp/container/vector) relies on [std::move_if_noexcept](https://en.cppreference.com/w/cpp/utility/move_if_noexcept) to choose between move and copy when the elements need to be relocated.

5、只有当struct有`ErrorMsg`成员的时候，才执行转换，对于没有`ErrorMsg`的struct，无需进行转换，显然这是遵循zero overhead principle的

```C++
template<typename RspFieldType>
struct RspFieldUnpackerBase : RspUnpackerBase {
private:
    using BaseClass = RspUnpackerBase;
protected:
    RspFieldUnpackerBase(CRspMsg &RspMsg) :
            BaseClass{RspMsg} {
        if (this->m_RspMsg.IsResponse()) {
            this->m_Rsp = reinterpret_cast<RspFieldType *>(this->m_RspMsg.m_lpMsgReader->GetBizFixed());
        }
    }

    RspFieldType *m_Rsp{nullptr};
public:
    /**
     * @brief 获得响应字段
     *
     * @return
     */
    RspFieldType *Get() {
        return this->m_Rsp;
    }

};

/**
 * @brief 对response field进行解包
 *
 * @tparam RspFieldType
 * @tparam Enable
 */
template<typename RspFieldType, typename Enable = void>
struct RspFieldUnpacker : RspFieldUnpackerBase<RspFieldType> {
private:
    using BaseClass = RspFieldUnpackerBase<RspFieldType>;
public:
    RspFieldUnpacker(CRspMsg &RspMsg) :
            BaseClass{RspMsg} {
    }
};

template<typename RspFieldType>
struct RspFieldUnpacker<RspFieldType, typename std::enable_if<
        has_member_ErrorMsg<RspFieldType>::value && has_member_ErrorID<RspFieldType>::value>::type>
        : virtual RspFieldUnpackerBase<RspFieldType> {
private:
    using BaseClass = RspFieldUnpackerBase<RspFieldType>;
public:
    RspFieldUnpacker(CRspMsg &RspMsg) :
            BaseClass{RspMsg} {
        /**
         * 只有在ErrorID 非0，即存在错误的情况下，才进行转换
         */
        if (m_RspField.ErrorID != 0) {
            /**
             * 需要进行一次deep copy
             */
            m_RspField = *(this->m_Rsp);
            int Ret = GbkToUtf8(this->m_Rsp->ErrorMsg, std::strlen(this->m_Rsp->ErrorMsg), m_RspField.ErrorMsg,
                                sizeof(m_RspField.ErrorMsg));
            if (Ret == 0) {
                LOG_DEBUG("GBK->UTF8转换成功");
            } else {
                LOG_ERROR("GBK->UTF8转换失败");
            }
            this->m_Rsp = &m_RspField;
        }
    }

private:
    RspFieldType m_RspField;
};

```



