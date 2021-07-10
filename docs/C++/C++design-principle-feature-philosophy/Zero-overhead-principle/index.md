# Zero-overhead principle

1、C++的design是遵循Zero-overhead principle，作为programmer，在开发的时候，也需要遵循zero overhead principle，即"You don't pay for what you don't use"

2、C++ Zero-overhead principle其实就是optimization principle

3、能够体现zero overhead的例子:

a、C++ data model: object的memory不会占用超过data member，C++没有额外导致它的扩大

b、

```C++
template<typename RspFieldType>
struct CRspFieldUnpackerBase: CRspUnpackerBase
{
private:
	using BaseClass = CRspUnpackerBase;
protected:
	CRspFieldUnpackerBase(CRspMsg &Msg) :
					BaseClass { Msg }
	{
		if (this->m_Msg.IsResponse()) // 如果不是正确应答，GetBizFixed已经调用过了
		{
			this->m_Rsp = reinterpret_cast<RspFieldType*>(this->m_Msg.m_lpMsgReader->GetBizFixed());
		}
	}
	RspFieldType *m_Rsp { nullptr };
public:
	/**
	 * @brief 获得响应字段
	 *
	 * @return
	 */
	RspFieldType* Get()
	{
		return this->m_Rsp;
	}
	std::ostream& Print(std::ostream &Stream) override
	{
		if (this->m_Rsp)
		{
			Stream << "消息体:" << m_Rsp << ";";
		}
		return Stream;
	}
};
/**
 * @brief 对response field进行解包
 *
 * @tparam RspFieldType
 * @tparam Enable
 */
template<typename RspFieldType, typename Enable = void>
struct CRspFieldUnpacker: CRspFieldUnpackerBase<RspFieldType>
{
private:
	using BaseClass = CRspFieldUnpackerBase<RspFieldType>;
public:
	CRspFieldUnpacker(CRspMsg &Msg) :
					BaseClass { Msg }
	{
	}
};

template<typename RspFieldType>
struct CRspFieldUnpacker<RspFieldType, typename std::enable_if<has_member_ErrorMsg<RspFieldType>::value && has_member_ErrorID<RspFieldType>::value>::type> : virtual CRspFieldUnpackerBase<RspFieldType>
{
private:
	using BaseClass = CRspFieldUnpackerBase<RspFieldType>;
public:
	CRspFieldUnpacker(CRspMsg &Msg) :
					BaseClass { Msg }
	{
		/**
		 * 只有在ErrorID 非0，即存在错误的情况下，才进行转换
		 */
		if (m_RspField.ErrorID != 0)
		{
			/**
			 * 需要进行一次deep copy
			 */
			m_RspField = *(this->m_Rsp);
			int Ret = ::GbkToUtf8(this->m_Rsp->ErrorMsg, std::strlen(this->m_Rsp->ErrorMsg), m_RspField.ErrorMsg, sizeof(m_RspField.ErrorMsg));
			if (Ret == 0)
			{
				LOG_DEBUG("GBK->UTF8转换成功");
			}
			else
			{
				LOG_ERROR("GBK->UTF8转换失败");
			}
			this->m_Rsp = &m_RspField;
		}
	}
private:
	RspFieldType m_RspField;
};
```



## cppreference [Zero-overhead principle](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle)



