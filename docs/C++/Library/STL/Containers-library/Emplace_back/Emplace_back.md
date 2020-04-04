# `emplace_back`

## [push_back vs emplace_back](https://stackoverflow.com/questions/4303513/push-back-vs-emplace-back)

## [Copy constructor and emplace_back() in C++](https://zpjiang.me/2018/08/08/Copy-Constructor-and-std-vector-emplace-back/)

## [Tip of the Week #112: emplace vs. push_back](https://abseil.io/tips/112)

这篇介绍地非常好

## `emplace_back`如何选择constructor？

### 20200404

今天碰到了这样的一个问题：

```c++
class CIStockInfoWrapper{
	CIStockInfoWrapper(int MktType, std::string Code)
			: m_MktType(MktType), m_Code(Code)
	{
		m_HQInfo = new HQInfo(m_Code, m_MktType);
	}
};

......
    
std::list<CIStockInfoWrapper> m_HQInfoList;
m_HQInfoList.emplace_back(MktType, std::to_string(i));
```

如果将`CIStockInfoWrapper(int MktType, std::string Code)`的说明修改为`CIStockInfoWrapper(int MktType, std::string& Code)`，则程序就编译报错：

```
hq_client_wrapper/mock_hq_accessor.h:73:63:   required from here
/usr/include/c++/4.8.2/bits/stl_list.h:114:71: 错误：对‘CIStockInfoWrapper::CIStockInfoWrapper(int&, std::basic_string<char>)’的调用没有匹配的函数
  : __detail::_List_node_base(), _M_data(std::forward<_Args>(__args)...)

./hq_client_wrapper/hqsvr_stock_info_wrapper.h:22:7: 附注： 备选需要 1 实参，但提供了 2 个
```

