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
ȡstr�ĵ�һ���ַ�
����0��ʾȡֵ����
����-1��ʾȡֵ�쳣��strΪ�յ�
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
 *��sSrc���ַ���sAt�ֳ������֣���������sAt��
 *���sSrc���ж೤sAtƥ�䣬�Ե�һ�γ���λ�ò��
 **/
EXPORT_API void FUNCTION_CALL_MODE split_half( const char * sSrc,const char * sAt,char * sDesStr1,char * sDesStr2 );
EXPORT_API int FUNCTION_CALL_MODE GetColString(char* sSrc,char* sDes,char ch ,int num);
EXPORT_API int FUNCTION_CALL_MODE GetColInteger(char * sstr, int *iDes, char ch, int num);
EXPORT_API int FUNCTION_CALL_MODE GetColDouble(char * sstr, double *enDes, char ch, int num);
EXPORT_API void FUNCTION_CALL_MODE split(string const& str, string const& delimiter, std::vector< std::string >* dest);

/*�ַ����滻���������ַ���s1�е����е�s2�滻Ϊs3*/
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
 * Description: �ַ����Ƚϣ����ǵ�s_libpublic���ͬ���������Ǹ������ᵼ��"\0"��"\0"�Ƚϻ᷵��-1��
 *              ��s_libpublic_uft�ﷵ�ص���0����ͳһ����0��ʹ�����������ӱ�������so������s_libpublic������
 * Input      : const char *d_str �ַ���
 *              const char *s_str �ַ���
 * Output     :
 * Return     : 0       �ɹ�
 *              -1      ����ΪNULL
 *              <>0     �ַ������
 * Author     : huhm
 * Date       : 2019/01/02
 ==========================================================*/
EXPORT_API int FUNCTION_CALL_MODE hs_strcmp(const char *d_str, const char *s_str);

//
EXPORT_API double FUNCTION_CALL_MODE string_to_double(std::string str);
//
EXPORT_API int FUNCTION_CALL_MODE string_to_int(std::string str);
//! ����ȥ����ǰ�ַ������ظ����ַ���
template<class T>
class CompareCharset
{
	set<T> m_setValue;//�������е��ַ�(��ʱ)
	typedef typename set<T>::iterator  ItSet;
public:
	//! ��ǰ�ַ����Ƿ����µ��ַ���
	// \return true true:���µ��ַ���
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
	//! �������ǰ���ַ���
	void clean()
	{
		m_setValue.clear();
	}
};



#endif
