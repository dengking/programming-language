#pragma once

#include <stddef.h>
#include <string>
#include <vector>

enum FiledMetaType
{
	FMT_INT = 0,
	FMT_STR,
	FMT_DOUBLE,
	FMT_CHAR,
	FMT_NIL
};

/**
 * 字段元信息
 */
struct SFieldMeta
{
	/**
	 * 字段名称
	 */
	const char *m_sFieldName;
	/**
	 * 字段在结构体中的偏移位置
	 */
	size_t m_iOffset;
	/**
	 * 字段类型
	 */
	enum FiledMetaType m_Type;
	/**
	 * 字段的长度
	 */
	size_t m_iSize;
};

/**
 * 结构体的运行时信息
 */
class CStructRtti
{
public:
	virtual ~CStructRtti()
	{
	}
	/**
	 * 字段元信息列表类型
	 */
	typedef std::vector<SFieldMeta> FieldMetaListT;
	/**
	 * 获得结构体名称
	 * @return
	 */
	virtual const std::string& GetName()=0;
	/**
	 * 获得字段元信息
	 * @return
	 */
	virtual const FieldMetaListT& GetFieldMeta()=0;
	/**
	 * 获得结构体所占内存大小
	 */
	virtual const int GetSize()=0;
	/**
	 * 获得结构体字段个数
	 */
	virtual int GetFieldCount()=0;
	/**
	 * 获得结构体第index字段的值，并将该值以字符串的形式返回
	 */
	virtual std::string GetValue(char* Data, int index)=0;
	/**
	 * 根据传入的字段名，获取该字段的索引值
	 */
	virtual int GetFieldIndex(const std::string& FieldName) = 0;
};
