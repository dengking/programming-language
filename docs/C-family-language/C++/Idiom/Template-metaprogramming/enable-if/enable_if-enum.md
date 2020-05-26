```c++
#include <type_traits>
#include <iostream>

/**
* 消息类型
*/
enum MsgType
{
	MSG_TRADE = 0x1,					/**>主机 交易 */
	MSG_QUERY = 0x2,					/**> 主机查询 */
};


/**
 */
struct MsgProtocol
{
	template<int ServiceMsgType>
	static auto Pack()->typename std::enable_if<ServiceMsgType==MsgType::MSG_TRADE, void>::type
	{
		std::cout<<"trade msg"<<std::endl;
	}
template<int ServiceMsgType>
	static auto Pack()->typename std::enable_if<ServiceMsgType==MsgType::MSG_QUERY, void>::type
	{
		std::cout<<"query msg"<<std::endl;
	}
};


int main()
{
MsgProtocol::Pack<MsgType::MSG_TRADE>();
MsgProtocol::Pack<MsgType::MSG_QUERY>();

}
```

