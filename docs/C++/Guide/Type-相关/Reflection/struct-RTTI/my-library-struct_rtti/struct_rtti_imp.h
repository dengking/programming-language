#pragma once

#include <stddef.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "struct_rtti.h"

/**
 * 结构体的运行时信息
 */
class CStructRttiImp: public CStructRtti
{
public:
	CStructRttiImp(FieldMetaListT&& Rtti, std::string&& Name, int Size)
			: m_FieldMetaList(Rtti), m_FieldCount(Rtti.size()), m_Name(Name), m_Size(Size)
	{
	}
	int GetFieldCount()
	{
		return m_FieldCount;
	}
	const std::string& GetName()
	{
		return m_Name;
	}
	const FieldMetaListT& GetFieldMeta()
	{
		return m_FieldMetaList;
	}
	const int GetSize()
	{
		return m_Size;
	}
	std::string GetValue(char* Data, int Index)
	{
		SFieldMeta& FieldMeta = m_FieldMetaList[Index];
		size_t iOffset = FieldMeta.m_iOffset;
		char *pField = (char*) Data + iOffset;
		switch (FieldMeta.m_Type)
		{
		case FMT_INT:
			{
			int Value = *((int*) pField);
			return std::to_string(Value);
		}
		case FMT_DOUBLE:
			{
			double Value = *((double*) pField);
			return std::to_string(Value);
		}
		case FMT_STR:
			{
			return std::string(pField);
		}
		case FMT_CHAR:
			{
			char Value = *(pField);
			return std::string(1, Value);
		}
		default:
			std::stringstream S;
			S << "不识别的字段类型：" << int(FieldMeta.m_Type) << "系统存在严重逻辑错误";
			throw std::logic_error(S.str());
		}
	}
	/**
	 * 获得字段的索引
	 * @param FieldName
	 * @return
	 */
	int GetFieldIndex(const std::string& FieldName)
	{
		int Index = 0;
		for(auto& Field:m_FieldMetaList)
		{
			if(FieldName == Field.m_sFieldName)
			{
				return Index;
			}
			++Index;
		}
		return -1;
	}
private:
	/**
	 * 结构体的大小
	 */
	int m_Size;
	/**
	 * 结构体的名称
	 */
	std::string m_Name;
	/**
	 * 字段个数
	 */
	int m_FieldCount;
	/**
	 * 字段的类型等信息
	 */
	FieldMetaListT m_FieldMetaList;
};

