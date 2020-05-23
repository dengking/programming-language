/*
 *  * test.cpp
 *   *
 *    *  Created on: 2020年4月23日
 *     *      Author: dengkai17334
 *      */

#include <string>
#include <iostream>

/**
 *  * 结构体1
 *   */
struct MyStruct1
{
};

/**
 *  * 结构体
 *   */
struct MyStruct2
{
};

/**
 *  * 结构体类型信息
 *   */
typedef std::string CStructRtti;

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct1* f)
{
	return "MyStruct1";
}

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct2* f)
{
	return "MyStruct2";
}

/**
 *  * 模板函数，根据结构体类型来获取结构体类型信息，它的实现是派发到上述的根据结构体对象来获取结构体类型休息
 *   * @return
 *    */
template<typename StructT>
CStructRtti GetRttiByType()
{
	StructT Tmp = StructT();
	return GetRtti(&Tmp);
}

class Utility
{
public:
	template<typename StrucT>
	static CStructRtti GetRtti()
	{
		return GetRttiByType<StrucT>();
	}

};
/**
 *  * 该namespace内部和外部的结构基本一致
 *   * @param f
 *    * @return
 *     */
namespace NS
{
/**
 *  * 结构体1
 *   */
struct MyStruct1
{
};

/**
 *  * 结构体
 *   */
struct MyStruct2
{
};

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct1* f)
{
	return "NS::MyStruct1";
}

/**
 *  * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct2* f)
{
	return "NS::MyStruct2";
}


class Utility
{
public:
	template<typename StrucT>
	static CStructRtti GetRtti()
	{
		return GetRttiByType<StrucT>();
	}

};
}

int main()
{
	std::cout<<Utility::GetRtti<MyStruct1>()<<std::endl;
	std::cout<<NS::Utility::GetRtti<NS::MyStruct1>()<<std::endl;
}

