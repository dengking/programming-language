#ifndef _STRING_UTILITY_H
#define _STRING_UTILITY_H
#include <Include/plugin_interface.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stdio.h>
#include "exports.h"

#ifndef _WIN32
#include <os/inttypes.h>
#include "algo_include/s_libpublic.h"
#endif

using namespace std;  

EXPORT_API std::string FUNCTION_CALL_MODE pchar_to_string(const char * pchar);

/*
取str的第一个字符
返回0表示取值正常
返回-1表示取值异常，str为空等
*/
EXPORT_API int FUNCTION_CALL_MODE string_to_char(const string &str,char &ch);
EXPORT_API void FUNCTION_CALL_MODE ToLowerString(char* pStr)  ;
EXPORT_API void FUNCTION_CALL_MODE ToLowerString(std::string &str) ;
EXPORT_API void FUNCTION_CALL_MODE ToUpperString(char* pStr)  ;
EXPORT_API void FUNCTION_CALL_MODE ToUpperString(std::string &str);
EXPORT_API std::string FUNCTION_CALL_MODE inttostr(int iSource);
EXPORT_API std::string FUNCTION_CALL_MODE doubletostr(double dSource);
EXPORT_API std::string FUNCTION_CALL_MODE longtostr(long dSource);
EXPORT_API std::string FUNCTION_CALL_MODE lltostr(long long dSource);
EXPORT_API std::string FUNCTION_CALL_MODE chartostr(char cSource);
EXPORT_API int FUNCTION_CALL_MODE GetSplitStrByParam(const char* sSrc,const char* sSplit,int iNum,char* sDest);
EXPORT_API int FUNCTION_CALL_MODE ReplaceXMLSpecialCharacter(char *sSrc);
EXPORT_API int FUNCTION_CALL_MODE ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr);
EXPORT_API void FUNCTION_CALL_MODE Ltrim(string &str);
EXPORT_API void FUNCTION_CALL_MODE Rtrim(string &str);
EXPORT_API void FUNCTION_CALL_MODE Trim(string &str);
EXPORT_API int FUNCTION_CALL_MODE CountSubstringInString(string &strSrc,string &strSub);
EXPORT_API vector<string> FUNCTION_CALL_MODE splitEx(const string& src, string separate_character);
EXPORT_API int FUNCTION_CALL_MODE Strstrex(const string & src,string separate_character);
/**
 *将sSrc按字符串sAt分成两部分，都不包含sAt。
 *如果sSrc中有多长sAt匹配，以第一次出现位置拆分
 **/
EXPORT_API void FUNCTION_CALL_MODE split_half( const char * sSrc,const char * sAt,char * sDesStr1,char * sDesStr2 );
EXPORT_API int FUNCTION_CALL_MODE GetColString(char* sSrc,char* sDes,char ch ,int num);
EXPORT_API int FUNCTION_CALL_MODE GetColInteger(char * sstr, int *iDes, char ch, int num);
EXPORT_API int FUNCTION_CALL_MODE GetColDouble(char * sstr, double *enDes, char ch, int num);
EXPORT_API void FUNCTION_CALL_MODE split(string const& str, string const& delimiter, std::vector< std::string >* dest);

/*字符串替换函数：将字符串s1中的所有的s2替换为s3*/
EXPORT_API void FUNCTION_CALL_MODE string_replace(string& s1,const string& s2,const string& s3);
EXPORT_API void FUNCTION_CALL_MODE replaceStr(char* pBuf, int bufLen, const string& s2,const string& s3);
#ifdef _WIN32
#define hs_strncpy strncpy
#define hs_strcpy strcpy
#define hs_snprintf snprintf
//#define hs_strcmp strcmp
#endif

/*==========================================================
 * Function   : hs_strcmp
 * Description: 字符串比较，覆盖掉s_libpublic里的同名函数，那个函数会导致"\0"与"\0"比较会返回-1，
 *              而s_libpublic_uft里返回的是0，现统一返回0，使用者优先链接本库所在so再链接s_libpublic来覆盖
 * Input      : const char *d_str 字符串
 *              const char *s_str 字符串
 * Output     :
 * Return     : 0       成功
 *              -1      输入为NULL
 *              <>0     字符不相等
 * Author     : huhm
 * Date       : 2019/01/02
 ==========================================================*/
EXPORT_API int FUNCTION_CALL_MODE hs_strcmp(const char *d_str, const char *s_str);

//
EXPORT_API double FUNCTION_CALL_MODE string_to_double(std::string str);
//
EXPORT_API int FUNCTION_CALL_MODE string_to_int(std::string str);
//! 用于去除当前字符集中重复的字符串
template<class T>
class CompareCharset
{
	set<T> m_setValue;//保存已有的字符(暂时)
	typedef typename set<T>::iterator  ItSet;
public:
	//! 当前字符串是否是新的字符串
	// \return true true:是新的字符串
	bool isNewStr(const T &inParam)
	{
		//if (inParam.empty())
		//{
		//	return false;
		//}

		pair<ItSet, bool> it = m_setValue.insert(inParam);
		return it.second;
	}
	size_t size()
	{
		return m_setValue.size();
	}
	//! 清理掉当前的字符集
	void clean()
	{
		m_setValue.clear();
	}
};



#endif
