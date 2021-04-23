
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <stdio.h>
#include "exports.h"

#ifndef _WIN32
#include <os/inttypes.h>

#endif

using namespace std;  

EXPORT_API std::string  pchar_to_string(const char * pchar);

/*
取str的第一个字符
返回0表示取值正常
返回-1表示取值异常，str为空等
*/
 int  string_to_char(const string &str,char &ch);
 void  ToLowerString(char* pStr)  ;
 void  ToLowerString(std::string &str) ;
 void  ToUpperString(char* pStr)  ;
 void  ToUpperString(std::string &str);
 std::string  inttostr(int iSource);
 std::string  doubletostr(double dSource);
 std::string  longtostr(long dSource);
 std::string  lltostr(long long dSource);
 std::string  chartostr(char cSource);
 int  GetSplitStrByParam(const char* sSrc,const char* sSplit,int iNum,char* sDest);
 int  ReplaceXMLSpecialCharacter(char *sSrc);
 int  ReplaceStr(char *sSrc, char *sMatchStr, char *sReplaceStr);
 void  Ltrim(string &str);
 void  Rtrim(string &str);
 void  Trim(string &str);
 int  CountSubstringInString(string &strSrc,string &strSub);
 vector<string>  splitEx(const string& src, string separate_character);
 int  Strstrex(const string & src,string separate_character);
/**
 *将sSrc按字符串sAt分成两部分，都不包含sAt。
 *如果sSrc中有多长sAt匹配，以第一次出现位置拆分
 **/
 void  split_half( const char * sSrc,const char * sAt,char * sDesStr1,char * sDesStr2 );
 int  GetColString(char* sSrc,char* sDes,char ch ,int num);
 int  GetColInteger(char * sstr, int *iDes, char ch, int num);
 int  GetColDouble(char * sstr, double *enDes, char ch, int num);
 void  split(string const& str, string const& delimiter, std::vector< std::string >* dest);

/*字符串替换函数：将字符串s1中的所有的s2替换为s3*/
 void  string_replace(string& s1,const string& s2,const string& s3);
 void  replaceStr(char* pBuf, int bufLen, const string& s2,const string& s3);
#ifdef _WIN32
#define hs_strncpy strncpy
#define hs_strcpy strcpy
#define hs_snprintf snprintf
//#define my_strcmp strcmp
#endif


 int  my_strcmp(const char *d_str, const char *s_str);

//
 double  string_to_double(std::string str);
//
 int  string_to_int(std::string str);
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
