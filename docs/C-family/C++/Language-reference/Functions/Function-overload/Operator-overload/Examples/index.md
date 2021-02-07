# Examples

本章章节overload operator的典型例子。

1、[joboccara](https://github.com/joboccara)/**[NamedType](https://github.com/joboccara/NamedType)**

2、[CPPTraits](https://github.com/macmade/CPPTraits)/[Unit-Tests](https://github.com/macmade/CPPTraits/tree/master/Unit-Tests)/[XS-TypeTraits.cpp](https://github.com/macmade/CPPTraits/blob/master/Unit-Tests/XS-TypeTraits.cpp)

3、overload `new` operator: 

a、[macmade](https://github.com/macmade)/**[CPP-ARC](https://github.com/macmade/CPP-ARC)**

## csdn [用C/C++实现一个日期类，重载运算符=，==，+，-，++，--，>，>=，<，<=等](https://blog.csdn.net/wangiijing/article/details/51472589)

```C++
#include<iostream>  
#include<windows.h>  
  
using namespace std;  
  
class Date  
{  
public:  
    Date(int year, int month, int day) //构造函数  
        :_year(year)  
        ,_month(month)  
        ,_day(day)  
    {}  
    Date(Date & d)   //拷贝构造  
        :_year(d._year)  
        , _month(d._month)  
        , _day(d._day)  
    {}  
    Date & operator = (const Date &d) //赋值运算符的重载  
    {//将实例d的所有成员变量值全部赋值给this,这里不存在指针问题，就不用考虑内存问题  
        _year = d._year;  
        _month = d._month;  
        _day = d._day;  
        return *this;  
    }  
    bool operator == (const Date& d)//重载 ==  
    {  
        return this->_year == d._year  
            && this->_month == d._month  
            && this->_day == d._day;  
    }  
    bool operator <(const Date& d) //重载 <  
    {  
        if (_year < d._year)//判断年  
        {  
            return true;  
        }  
        else if (_year == d._year)//年相等，就判断月  
        {  
            if (_month<d._month)  
            {  
                return true;  
            }  
            else if (_month == d._month)//月相等，判断天  
            {  
                if (_day<d._day)  
                {  
                    return true;  
                }  
            }  
        }  
        return false;  
    }  
    bool operator <=(const Date& d)//重载<=(复用函数<和==)  
    {  
        return (*this<d) || (*this == d);//当<或==满足一个的时候为真  
    }  
    bool operator >(const Date& d)//重载<=(复用函数<=)  
    {  
        return !(*this <= d);  
    }  
    bool operator >=(const Date& d)  
    {  
        return !(*this < d);  
    }  
    Date operator+ (int day)//重载+  
    {  
        int ret = GetMonthDay(_year, _month);//调用函数获取当年当月天数  
        Date tmp = *this;//用this创建一个临时对象  
        while ((_day + day) > ret)//循环条件：当总天数大于一个月的天数时  
        {  
            if ((tmp._month + 1) > 12)//当月份大于12时  
            {  
                tmp._year++;//年份加一  
                tmp._month = 0;//月份置零  
            }  
            else//否则，月份加一  
            {  
                tmp._month++;  
            }  
            day -= ret;//总天数减去当月的天数  
            ret = GetMonthDay(_year, tmp._month);//获取下一月的天数  
        }  
        tmp._day += day;  
        return tmp;  
    }  
    Date& operator+= (int day)//重载+=（复用+）  
    {  
        *this = *this + day;  
        return *this;  
    }  
  
    Date operator- (int day)//重载-  
    {  
        int ret = GetMonthDay(_year, _month);  
        Date tmp = *this;  
        while (day > tmp._day)//当需要减的天>当月的天数时进行循环  
        {  
            while (day > ret)  
            {  
                if (tmp._month > 1)  
                {  
                    tmp._month--;  
                }  
                else  
                {  
                    tmp._year--;  
                    tmp._month = 11;  
                }  
                day -= ret;  
                ret = GetMonthDay(tmp._year, tmp._month);  
            }  
            day -= ret;  
        }  
        tmp._day -= day;  
        return tmp;  
    }  
    Date& operator-= (int day)//重载-=（复用-）  
    {  
        *this = *this - day;  
        return *this;  
    }  
  
    Date operator++()  
    {  
        return *this += 1;  
    }  
    Date operator++(int)  
    {  
        return *this + 1;  
    }  
  
    Date operator--()  
    {  
        return *this -= 1;  
    }  
    Date operator--(int)  
    {  
        return *this - 1;  
    }  
  
    int operator-(const Date& d)//日期间日期  
    {  
        Date tmp = *this;  
        int day = 0;  
        int ret = 0;  
        if (tmp > d)  
        {  
            while (tmp._year > d._year)  
            {  
                if (tmp._month > 2)  
                {  
                    if (IsLeapYear(tmp._year))  
                    {  
                        day += 366;  
                        tmp._year--;  
                    }  
                }  
                else  
                {  
                    day += 365;  
                    tmp._year--;  
                }  
            }  
            while (tmp._month > d._month)  
            {  
                ret = GetMonthDay(tmp._year, tmp._month);  
                day += ret;  
            }  
            if (tmp._day > d._day)  
            {  
                day += tmp._day - d._day;  
            }  
            else  
            {  
                day -= d._day - tmp._day;  
            }  
  
        }  
        return day;  
    }  
    void Display()  
    {  
        cout << _year << "\t" << _month << "\t" << _day << endl;  
    }  
protected:  
    bool IsLeapYear(int year)//判断是否为闰年  
    {  
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)  
        {  
            return true;  
        }  
        return false;  
    }  
    int GetMonthDay(int year, int month)//获得当年当月天数  
    {  
        int monthArray[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };  
  
        int day = monthArray[month];  
  
        if (month == 2 && IsLeapYear(year))  
        {  
            day += 1;  
        }  
        return day;  
    }  
private:  
    int _year;  
    int _month;  
    int _day;  
};  
```
