# Dispatch based on constant

关于constant，参见`C++\Language-reference\Expressions\Constant-expressions`。

实现方式有:

1) enum dispatch 

## enum dispatch / enum in TMP

我记得在TMP中，有多处是使用到了enum的，需要整理一下。

### Enum dispatch: Enum + `enable_if`

为每个trait class添加一个表示其类型的常量，然后添加判断条件来判断它属于哪种类型？


比如我定义有三种类型：

```C++
// 服务类型
enum EServiceType
{
    
   TradeService=1;//交易类服务 
   QueryService=2;//查询类服务
};
// 某个服务
struct FooService
{
	constexpr static int ServiceType = EServiceType::TradeService;
};
// 判断是否是交易类服务
constexpr bool IsTradeService(EServiceType ServiceType)
{
    return ServiceType == TradeService;
}

template<class ServiceTraitType>
auto ReqService()->typename std::enable_if<IsTradeService(ServiceTraitType::ServiceType), int>::type
{
    
}
```





### 我的实践

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

