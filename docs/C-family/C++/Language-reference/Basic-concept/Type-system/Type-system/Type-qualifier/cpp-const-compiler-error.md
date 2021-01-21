# const的一些使用记录
```
	std::string to_string() const {
		return std::to_string(year_) + std::to_string(month_) + ":"
				+ std::to_string(day_);
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, const TradeDate* p_obj) {
		out << "交易时间(" << p_obj->to_string() << ")";
		return out;
	}
	/// std output
	friend std::ostream& operator<<(std::ostream &out, const TradeDate& obj) {
		out << "交易时间(" << obj.to_string() << ")";
		return out;
	}
```
在`operator<<`中，入参`obj`和`p_obj`都是`const`，所以它们调用的函数也只能够是`const`的，如果函数`to_string()`不是`const`，且编译的时候加上了`-fpermissive`，则会导致编译报错，报错信息如下:
```
../core/trade_date_time.h:47:42: 错误：将‘const TradeDate’作为‘std::string TradeDate::to_string()’的‘this’实参时丢弃了类型限定 [-fpermissive]
   out << "交易时间(" << p_obj->to_string() << ")";
```