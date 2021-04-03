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





#### 我的实践

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



### Enum specialization

gist [kimgea](https://gist.github.com/kimgea)/**[cpp-traits.cpp](https://gist.github.com/kimgea/7197a90f2fd81945e5baf123537b2072)**

```C++
#include <vector>
#include <list>
#include <set>


enum class insert_method_enum{general, push_back, push_front};


// Main template insert helper with its temlate specialications
template<insert_method_enum i>
struct insert_selector
{
	template<typename t, typename v>
	static void insert(t& object, v value)
	{
		object.insert(object.end(), value);
	}
};

template<>
struct insert_selector<insert_method_enum::push_back>
{
	template<typename t, typename v>
	static void insert(t& object, v value)
	{
		object.push_back(value);
	}
};
template<>
struct insert_selector<insert_method_enum::push_front>
{
	template<typename t, typename v>
	static void insert(t& object, v value)
	{
		object.push_front(value);
	}
};


// type traits
template<typename t, typename v>
struct insert_type {
	static const insert_method_enum value = insert_method_enum::general;
};

template<typename v>
struct insert_type<std::vector<v>, v> {
	static const insert_method_enum value = insert_method_enum::push_back;
};

template<typename v>
struct insert_type<std::list<v>, v> {
	static const insert_method_enum value = insert_method_enum::push_front;
};



// Function used by end user
template<typename t, typename v>
void insert(t& object, v value)
{
	insert_selector<insert_type<t, v>::value>::insert(object, value);
}

// Extra function to make the end user able to overwrite pre made specialications
template<insert_method_enum i, typename t, typename v>
void insert(t& object, v value)
{
	insert_selector<i>::insert(object, value);
}


int main()
{
	std::vector<int> vector;
	insert(vector, 1);	// Specialized - vector.push_back(1)


	std::list<int> list;
	insert(list, 1);	// Specialized - list.push_front(1)

	std::set<int> set;
	insert(set, 1);	// General - set.insert(set.end(), 1);	



	std::list<int> list2;
	insert<insert_method_enum::push_back>(list2, 1);	

	return 0;
}
// g++ --std=c++11 test.cpp

```

