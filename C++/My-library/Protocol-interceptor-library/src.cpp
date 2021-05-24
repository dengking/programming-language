
struct CRspMsg
{
	CRspMsg(IMsgReader *lpMsgReader, EServerType Source = EServerType::Front) :
					m_lpMsgReader(lpMsgReader), m_Source(Source), m_Head { m_lpMsgReader->GetHead() }
	{
		m_bIsResponse = this->IsResponse();
	}

	bool IsValid()
	{
		return m_lpMsgReader != nullptr;
	}
	/**
	 * @brief 
	 *
	 * @return
	 */
	bool IsResponse()
	{
		return Msg::IsResponse(m_Head);
	}
	/**
	 * @brief 
	 *
	 * @return
	 */
	inline bool IsSuccess()
	{
		return this->m_bIsResponse;
	}
	bool IsErrorRsp()
	{
		return Msg::IsErrorRsp(m_Head);
	}
	FuncNoType GetFuncNo()
	{
		return m_Head->FunctionID;
	}

	SenderIDType GetSenderID()
	{
		return m_Head->UserDefined;
	}
	UstTokenType GetToken()
	{
		return m_Head->Token;
	}
	IMsgReader *m_lpMsgReader { nullptr };
	bool m_bIsResponse { false };
	EServerType m_Source;
	Head *m_Head { nullptr };

	std::ostream& Print(std::ostream &Stream)
	{
		if (this->IsValid())
		{
			Stream << "消息头:" << "FunctionId=" << this->GetFuncNo() << ";";
			if (this->IsSuccess())
			{
				Stream << "成功响应;";
			}
			else
			{
				Stream << "错误响应;";
			}
		}
		return Stream;
	}
	friend std::ostream& operator<<(std::ostream &Stream, CRspMsg *Msg)
	{
		return Msg->Print(Stream);
	}
	friend std::ostream& operator<<(std::ostream &Stream, CRspMsg &Msg)
	{
		return Msg.Print(Stream);
	}
};

#pragma region Unpacker///////////////////////////////////////////////////////////////////////////////////////
define_has_member(ErrorMsg);

define_has_member(ErrorID);

struct CRspUnpackerBase
{
	CRspUnpackerBase(CRspMsg &Msg) :
					m_Msg(Msg)
	{

	}
	virtual ~CRspUnpackerBase() = default;
	/**
	 * @brief 
	 *
	 * @return
	 */
	inline bool IsSuccess()
	{
		return m_Msg.IsSuccess();
	}
	/**
	 * @brief 
	 *
	 * @return
	 */
	inline FuncNoType GetFuncNo()
	{
		return m_Msg.GetFuncNo();
	}
	/**
	 * @brief 
	 *
	 * @return
	 */
	inline SenderIDType GetSenderID()
	{
		return m_Msg.GetSenderID();
	}

	/**
	 * @brief 输出对象
	 *
	 * @param Stream
	 * @return
	 */
	virtual std::ostream& Print(std::ostream &Stream)=0;
	/**
	 * @brief 获得字符串表示
	 *
	 * @return
	 */
	std::string Str()
	{
		std::ostringstream S;
		S << this;
		return S.str();
	}
protected:
	CRspMsg &m_Msg;
};
static inline std::ostream& operator<<(std::ostream &Stream, CRspUnpackerBase *Unpacker)
{
	return Unpacker->Print(Stream);
}
static inline std::ostream& operator<<(std::ostream &Stream, CRspUnpackerBase &Unpacker)
{
	return Unpacker.Print(Stream);
}

struct CRspInfoFieldUnpacker: CRspUnpackerBase
{
private:
	using BaseClass = CRspUnpackerBase;
public:
	CRspInfoFieldUnpacker(CRspMsg &Msg) :
					BaseClass { Msg }
	{
		this->Unpack();
	}
	CHSInsRspInfoField& Get()
	{
		return m_lpRspInfoField;
	}
	std::ostream& Print(std::ostream &Stream) override
	{
		if (this->m_Msg.IsSuccess())
		{
			Stream << "响应信息:" << &m_lpRspInfoField << ";";
		}
		return Stream;
	}

protected:
	/**
	 * 应答信息
	 */
	CHSInsRspInfoField m_lpRspInfoField { }; // 使用zero initialization
private:
	void Unpack()
	{
		if (this->m_Msg.IsResponse())
		{
			m_lpRspInfoField.ErrorID = 0;
			CStrHelper::hs_strncpy(m_lpRspInfoField.ErrorMsg, "No Error", sizeof(m_lpRspInfoField.ErrorMsg));
		}
		else if (this->m_Msg.IsErrorRsp())
		{

			CRspInfoField *lpRspError = reinterpret_cast<CRspInfoField*>(this->m_Msg.m_lpMsgReader->GetBizFixed());
			if (lpRspError->ErrorID != 0)
			{
				m_lpRspInfoField.ErrorID = lpRspError->ErrorID;
				int Ret = ::GbkToUtf8(lpRspError->ErrorMsg, std::strlen(lpRspError->ErrorMsg), m_lpRspInfoField.ErrorMsg, sizeof(m_lpRspInfoField.ErrorMsg));
				if (Ret == 0)
				{
					LOG_DEBUG("GBK->UTF8转换成功");
				}
				else
				{
					LOG_ERROR("GBK->UTF8转换失败");
				}
			}
		}
		else
		{
			m_lpRspInfoField.ErrorID = -1;
			CStrHelper::strncpy(m_lpRspInfoField.ErrorMsg, "Msg Error", sizeof(m_lpRspInfoField.ErrorMsg));
		}
	}

};

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

template<typename RspFieldType>
struct CQueryRspFieldUnpackerBase: CRspUnpackerBase
{
private:
	using BaseClass = CRspUnpackerBase;
protected:
	CQueryRspFieldUnpackerBase(CRspMsg &Msg) :
					BaseClass { Msg }
	{
		if (this->m_Msg.IsResponse())
		{
			m_BizHead = reinterpret_cast<SBusinessQueryResponseHead*>(this->m_Msg.m_lpMsgReader->GetBizHead());
			m_IsFinished = !((m_BizHead->cFlags) & 0b10000000);
			m_PositionSn = m_BizHead->iPositionSn;
		}
	}
	/**
	 * @brief 打印业务头
	 *
	 * @param Stream
	 * @return
	 */
	std::ostream& PrintBizHead(std::ostream &Stream)
	{
		if (this->m_Msg.IsSuccess())
		{
			Stream << "IsFinished=" << m_IsFinished << "PositionSn=" << m_PositionSn << ";";
		}
		return Stream;
	}
	/**
	 * @brief 打印业务体
	 *
	 * @param Stream
	 * @return
	 */
	std::ostream& PrintMsgBody(std::ostream &Stream)
	{
		if (this->m_Msg.IsSuccess())
		{
			int i = 0;
			for (auto &Rsp : m_RspFileds)
			{
				Stream << "第" << i << "条记录:" << Rsp; // 访问第i条记录
				++i;
			}
		}
		return Stream;
	}
public:
	RspFieldType* operator[](std::size_t Index)
	{
		return m_RspFileds[Index];
	}
	std::size_t GetRowCount()
	{
		return m_RspFileds.size();
	}
	/**
	 * 是否完成查询
	 * @return
	 */
	bool IsFinished()
	{
		return m_IsFinished;
	}
	PositionSnType GetPositionSn()
	{
		return m_PositionSn;
	}
	std::ostream& Print(std::ostream &Stream) override
	{
		return PrintMsgBody(PrintBizHead(Stream));
	}


protected:
	std::vector<RspFieldType*> m_RspFileds; //m_RspFileds中的一些数据可能指向m_RspFileds_Copy中的成员
	SBusinessQueryResponseHead *m_BizHead { nullptr };
	/**
	 * 表示是否查询完成
	 */
	bool m_IsFinished { true };
	PositionSnType m_PositionSn { 0 };
};

template<typename RspFieldType, typename Enable = void>
struct CQueryRspFieldUnpacker: CQueryRspFieldUnpackerBase<RspFieldType>
{
private:
	using BaseClass = CQueryRspFieldUnpackerBase<RspFieldType>;
public:
	CQueryRspFieldUnpacker(CRspMsg &Msg) :
					BaseClass { Msg }
	{
		this->Unpack();
	}
private:
	void Unpack()
	{
		if (!this->m_Msg.IsResponse())	// 如果不是正确应答，没有业务头，业务体是统一的错误消息格式，且GetBizFixed已经调用过了
		{
			return;
		}
		if (this->m_BizHead->uResponseNumber != 0)
		{
			RspFieldType *lpBizFixed = nullptr;
			while (!(this->m_Msg.m_lpMsgReader->IsEof()))
			{
				lpBizFixed = reinterpret_cast<RspFieldType*>(this->m_Msg.m_lpMsgReader->GetBizFixed());
				this->m_RspFileds.push_back(lpBizFixed);
				this->m_Msg.m_lpMsgReader->NextLine();
			}
		}
	}
};

template<typename RspFieldType>
struct CQueryRspFieldUnpacker<RspFieldType, typename std::enable_if<has_member_ErrorMsg<RspFieldType>::value && has_member_ErrorID<RspFieldType>::value>::type> : CQueryRspFieldUnpackerBase<RspFieldType>
{
private:
	using BaseClass = CQueryRspFieldUnpackerBase<RspFieldType>;
public:
	CQueryRspFieldUnpacker(CRspMsg &Msg) :
					BaseClass { Msg }
	{
		this->Unpack();
	}
private:
	void Unpack()
	{
		if (!this->m_Msg.IsResponse())	// 如果不是正确应答，没有业务头，业务体是统一的错误消息格式，且GetBizFixed已经调用过了
		{
			return;
		}

		if (this->m_Msg.m_BizHead->uResponseNumber != 0)
		{
			RspFieldType *lpBizFixed = nullptr;
			while (!(this->m_Msg.m_lpMsgReader->IsEof()))
			{
				lpBizFixed = reinterpret_cast<RspFieldType*>(this->m_Msg.m_lpMsgReader->GetBizFixed());
				if (lpBizFixed->ErrorID != 0)
				{
					this->m_RspFileds_Copy.push_back(*lpBizFixed);
					RspFieldType &Copy = this->m_RspFileds_Copy.back();
					int Ret = ::GbkToUtf8(lpBizFixed->ErrorMsg, std::strlen(lpBizFixed->ErrorMsg), Copy.ErrorMsg, sizeof(Copy.ErrorMsg));
					if (Ret == 0)
					{
						LOG_DEBUG("GBK->UTF8转换成功");
					}
					else
					{
						LOG_ERROR("GBK->UTF8转换失败");
					}
					this->m_RspFileds.push_back(&Copy);
				}
				else
				{
					this->m_RspFileds.push_back(lpBizFixed);
				}
				this->m_Msg.m_lpMsgReader->NextLine();
			}
		}
	}
	std::vector<RspFieldType> m_RspFileds_Copy;
};

/**
 * 反序列化器、解包器，对响应消息进行解包，并进行反序列化
 */
template<typename RspFieldType>
struct CMsgUnpacker: CRspUnpackerBase
{
public:
	CMsgUnpacker(CRspMsg &Msg) :
					CRspUnpackerBase { Msg }, m_RspInfoFieldUnpacker { Msg }, m_RspFieldUnpacker { Msg }
	{
	}

	virtual ~CMsgUnpacker() = default;

	CRspInfoField* GetRspInfoField()
	{
		return &m_RspInfoFieldUnpacker.Get();
	}
	RspFieldType* GetRspField()
	{
		return m_RspFieldUnpacker.Get();
	}

	std::ostream& Print(std::ostream &Stream) override
	{
		Stream << this->m_Msg << m_RspInfoFieldUnpacker << m_RspFieldUnpacker;
		return Stream;
	}
protected:
	CRspInfoFieldUnpacker m_RspInfoFieldUnpacker;
	CRspFieldUnpacker<RspFieldType> m_RspFieldUnpacker;

};

/**
 * 消息类型为MsgType::MSG_NO_INDEX的消息的反序列化器
 */
template<typename RspFieldType>
struct CNoBizHeadMsgUnpacker: CMsgUnpacker<RspFieldType>
{
	CNoBizHeadMsgUnpacker(CRspMsg &Msg) :
					CMsgUnpacker<RspFieldType>(Msg)
	{
	}
};

/**
 * 消息类型为MsgType::MSG_QUERY的消息的反序列化器
 */
template<typename RspFieldType>
struct CQueryMsgUnpacker: CRspUnpackerBase
{
public:
	CQueryMsgUnpacker(CRspMsg &Msg) :
					CRspUnpackerBase { Msg }, m_RspInfoFieldUnpacker { Msg }, m_QueryRspFieldUnpacker { Msg }
	{
	}
	std::ostream& Print(std::ostream &Stream) override
	{
		Stream << this->m_Msg << m_RspInfoFieldUnpacker << m_QueryRspFieldUnpacker;
		return Stream;
	}
	CRspInfoField* GetRspInfoField()
	{
		return &m_RspInfoFieldUnpacker.Get();
	}
	/**
	 * 是否完成查询
	 * @return
	 */
	bool IsFinished()
	{
		return m_QueryRspFieldUnpacker.IsFinished();
	}
	PositionSnType GetPositionSn()
	{
		return m_QueryRspFieldUnpacker.GetPositionSn();
	}

	/**
	 * 访问第Index条记录
	 * @param Index
	 * @return
	 */
	RspFieldType* operator[](unsigned int Index)
	{
		return m_QueryRspFieldUnpacker[Index];
	}

	/**
	 * 结果集的行数
	 * @return
	 */
	int GetRowCount()
	{
		return m_QueryRspFieldUnpacker.GetRowCount();
	}

protected:
	CRspInfoFieldUnpacker m_RspInfoFieldUnpacker;
	CQueryRspFieldUnpacker<RspFieldType> m_QueryRspFieldUnpacker;
};
