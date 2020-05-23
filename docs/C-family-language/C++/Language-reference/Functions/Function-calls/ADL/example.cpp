/*
 *  * test.cpp
 *   *
 *    *  Created on: 2020��4��23��
 *     *      Author: dengkai17334
 *      */

#include <string>
#include <iostream>

/**
 *  * �ṹ��1
 *   */
struct MyStruct1
{
};

/**
 *  * �ṹ��
 *   */
struct MyStruct2
{
};

/**
 *  * �ṹ��������Ϣ
 *   */
typedef std::string CStructRtti;

/**
 *  * ��ýṹ��MyStruct��������Ϣ�����ǰ��ն�������ȡ�ṹ��������Ϣ
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct1* f)
{
	return "MyStruct1";
}

/**
 *  * ��ýṹ��MyStruct��������Ϣ�����ǰ��ն�������ȡ�ṹ��������Ϣ
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct2* f)
{
	return "MyStruct2";
}

/**
 *  * ģ�庯�������ݽṹ����������ȡ�ṹ��������Ϣ������ʵ�����ɷ��������ĸ��ݽṹ���������ȡ�ṹ��������Ϣ
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
 *  * ��namespace�ڲ����ⲿ�Ľṹ����һ��
 *   * @param f
 *    * @return
 *     */
namespace NS
{
/**
 *  * �ṹ��1
 *   */
struct MyStruct1
{
};

/**
 *  * �ṹ��
 *   */
struct MyStruct2
{
};

/**
 *  * ��ýṹ��MyStruct��������Ϣ�����ǰ��ն�������ȡ�ṹ��������Ϣ
 *   * @param f
 *    * @return
 *     */
CStructRtti GetRtti(MyStruct1* f)
{
	return "NS::MyStruct1";
}

/**
 *  * ��ýṹ��MyStruct��������Ϣ�����ǰ��ն�������ȡ�ṹ��������Ϣ
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

