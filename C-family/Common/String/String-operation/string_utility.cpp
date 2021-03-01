#include "string_utility.h"
#include <cctype>   
#include <string.h>   
#include <algorithm>   
#include <sstream>
#include <iomanip>
#include <limits.h>
#include <functional>
#include <stdlib.h>
using namespace std;  

std::string FUNCTION_CALL_MODE pchar_to_string(const char * pchar)
{
    if (NULL == pchar) return "";

    return std::string(pchar); 
}

int FUNCTION_CALL_MODE string_to_char(const string &str,char &ch)
{
  ch = ' ';
  if (str.empty()) return -1;

  ch = str.at(0);

  return 0;
}

void FUNCTION_CALL_MODE ToUpperString(string &str)
{  
    transform(str.begin(), str.end(), str.begin(), (int (*)(int))toupper);
}  
void FUNCTION_CALL_MODE ToUpperString(char* pStr)
{
	unsigned int i;
	for(i=0;i<strlen(pStr);i++) 
	{
		if ( pStr[i] >96 && pStr[i] < 123 ) 
			pStr[i] = (char)toupper(pStr[i]); 
	}
}

void FUNCTION_CALL_MODE ToLowerString(string &str)
{  
   transform(str.begin(), str.end(), str.begin(), (int (*)(int))tolower);  
} 

void FUNCTION_CALL_MODE ToLowerString(char* pStr)
{  
  unsigned int i;
  for(i=0;i<strlen(pStr);i++) 
  {
    if ( pStr[i] >64 && pStr[i] < 91 ) 
      pStr[i] = (char)tolower(pStr[i]); 
  }
} 

std::string FUNCTION_CALL_MODE inttostr(int iSource)
{
  stringstream ss;
  ss.str("");
  ss<< setiosflags(ios::fixed) << iSource;
  return ss.str();
}

std::string FUNCTION_CALL_MODE doubletostr(double dSource)
{
	stringstream ss;
	ss.str("");
	ss<< setiosflags(ios::fixed) << dSource;
	return ss.str();
}

std::string FUNCTION_CALL_MODE longtostr(long dSource)
{
	stringstream ss;
	ss.str("");
	ss<< setiosflags(ios::fixed) << dSource;
	return ss.str();
}

std::string FUNCTION_CALL_MODE lltostr(long long dSource)
{
	stringstream ss;
	ss.str("");
	ss << setiosflags(ios::fixed) << dSource;
	return ss.str();
}

std::string FUNCTION_CALL_MODE chartostr(char cSource)
{
  stringstream ss;
  ss.str("");
  ss<< cSource;
  return ss.str();
}
/*取分割符分隔出来的字符串*/
/*sSrc=aaa|bbbb|ccc,sPlit='|',iNum=1,则sDest=bbbb*/
int FUNCTION_CALL_MODE GetSplitStrByParam(const char* sSrc,const char* sSplit,int iNum,char* sDest)
{
  char sSrcStr[1024]={0};
  strncpy(sSrcStr,sSrc,sizeof(sSrcStr));
  char * p;
  char *outer_ptr = NULL;
  int i=0;
  p = strtok_r (sSrcStr,sSplit,&outer_ptr);
  if (p==NULL)
  {
    strcpy(sDest,sSrc);
    return 0;
  }
  while(p!=NULL && i<=iNum) { 
    if (i==iNum)
    {
      strcpy(sDest,p);
    }
    p = strtok_r(NULL,sSplit,&outer_ptr);
    i++;
  } 
  return 0;
}
// 替换字符串中特征字符串为指定字符串
int FUNCTION_CALL_MODE ReplaceStr(char *sSrc, const char *sMatchStr, const char *sReplaceStr)
{
  size_t  StringLen;
  char caNewString[102400]={0};

  char *FindPos = strstr(sSrc, sMatchStr);
  if( (!FindPos) || (!sMatchStr) )
    return -1;

  while( FindPos )
  {
    memset(caNewString, 0, sizeof(caNewString));
    StringLen = FindPos - sSrc;
    strncpy(caNewString, sSrc, StringLen);
    strncat(caNewString, sReplaceStr,sizeof(sReplaceStr));
    strncat(caNewString, FindPos + strlen(sMatchStr),sizeof(caNewString));
    strncpy(sSrc, caNewString,sizeof(caNewString));

    FindPos = strstr(sSrc, sMatchStr);
  }

  return 0;
}
int FUNCTION_CALL_MODE ReplaceXMLSpecialCharacter(char *sSrc)
{
  ReplaceStr(sSrc,">","&gt;");
  ReplaceStr(sSrc,"<","&lt;");
  //ReplaceStr(sSrc,"''","&apos;");
  ReplaceStr(sSrc,"\"","&quot;");
  //ReplaceStr(sSrc,"&","&amp;");
  return 0;
}

void FUNCTION_CALL_MODE Ltrim(string &str)
{
  str.erase(str.begin(), find_if(str.begin(), str.end(),not1(ptr_fun(::isspace))));
}
void FUNCTION_CALL_MODE Rtrim(string &str)
{
  str.erase(find_if(str.rbegin(), str.rend(),not1(ptr_fun(::isspace))).base(),str.end());
}
void FUNCTION_CALL_MODE Trim(string &str)
{
  Ltrim(str);
  Rtrim(str);
}
int FUNCTION_CALL_MODE CountSubstringInString(string &strSrc,string &strSub)
{
  size_t i=0;
  int count = 0;
  while(strSrc.find(strSub,i)!=strSrc.npos)
  {
    i = strSrc.find(strSub,i) + 1;
    count++;
  }
  return count;
}
vector<string> FUNCTION_CALL_MODE splitEx(const string& src, string separate_character)
{
  vector<string> strs;
  size_t separate_characterLen = separate_character.size();//分割字符串的长度,这样就可以支持如“,,”多字符串的分隔符支
  size_t lastPosition = 0,index = std::string::npos;
  while (src.npos != (index = src.find(separate_character,lastPosition)))
  {
    strs.push_back(src.substr(lastPosition,index - lastPosition));
    lastPosition = index + separate_characterLen;
  }
  string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容
  if (!lastString.empty())
    strs.push_back(lastString);//如果最后一个分隔符后还有内容就入队
  return strs;
}

int FUNCTION_CALL_MODE Strstrex(const string& src, string separate_character) {
	string sMainStr = src;
	if(NULL == strstr(sMainStr.c_str(),separate_character.c_str()))
	{
		return -1;
	}
	return 0;
}

void FUNCTION_CALL_MODE split_half( const char * sSrc,const char * sAt,char * sDesStr1,char * sDesStr2 )
{
    size_t pex = strcspn(sSrc,sAt);
    if(0 >= pex) return;
    strxfrm(sDesStr1,sSrc,pex);sDesStr1[pex] = '\0';
    strcpy(sDesStr2,sSrc+pex+strlen(sAt));
}

/*
  GetColString: 取得指定字符串中按照指定分隔符分隔得指定部分,按整数返回
  sSrc: 源串
  iDes: 返回值,整型
  ch : 分隔字符
  num: 取第几部分
*/
int FUNCTION_CALL_MODE GetColInteger(char * sstr, int *iDes, char ch, int num)
{
  char buff[100];
  unsigned int i=0;

  memset(buff, 0, 100);
  if (GetColString(sstr, buff, ch, num)==0)
  {
    for (i=0; i< strlen(buff); i++)
    {
      if ((buff[i] >= '0') && (buff[i] <= '9'))
        continue;
      else if ( (i==0) && ((buff[i] == '-')||(buff[i] == '+')) )
        continue;
      else
        return -2;
    }
    *iDes = atoi(buff);
    if (*iDes == INT_MAX || *iDes == INT_MIN)
    {
      return -2;
    }
    return 0;
  }
  else
  {
    return -2;
  }
}

/*
  GetColString: 取得指定字符串中按照指定分隔符分隔得指定部分,按浮点值返回
  sSrc: 源串
  enDes: 返回值,实型
  ch : 分隔字符
  num: 取第几部分
*/
int FUNCTION_CALL_MODE GetColDouble(char * sstr, double *enDes, char ch, int num)
{
  char buff[100];
  unsigned int i=0;
  int ipointcnt=0;

  memset(buff, 0, 100);
  if (GetColString(sstr, buff, ch, num)==0)
  {
    for (i=0; i< strlen(buff); i++)
    {
      if (buff[i] == '.')
      {
        ipointcnt++;
      }
      else if ((buff[i] >= '0') && (buff[i] <= '9'))
        continue;
      else if ( (i==0) && ((buff[i] == '-') || (buff[i] == '+')))
        continue;
      else
        return -2;
    }

    if (ipointcnt > 1)
    {
      return -2;
    }
    *enDes = atof(buff);

    return 0;
  }
  else
  {
    return -2;
  }
}
/*
  GetColString: 取得指定字符串中按照指定分隔符分隔得指定部分,按字符串返回
  sSrc: 源串
  sDes: 返回值,目标串
  ch : 分隔字符
  num: 取第几部分
*/
int FUNCTION_CALL_MODE GetColString(char* sSrc,char* sDes,char ch ,int num)
{
  char *pCurr = sSrc;
  char *pNext;
  int i=0;
  for(i=0; i<num; i++)
  {
    if ((pCurr = strchr(pCurr,ch))==NULL)
    {
      break;
    }
    if ( pCurr[0] == ch )
    {
      pCurr++;
    }
  }

  if (!pCurr)
  {
    *sDes = 0;
  }
  else
  {
    pNext = strchr(pCurr,ch);
    if (!pNext)
    {
      strcpy(sDes, pCurr);
    }
    else
    {
      strncpy(sDes, pCurr, (size_t)(pNext - pCurr));
      sDes[pNext - pCurr] = 0;
    }
  }

  if(strlen(sDes)||(i==num && strchr(sSrc,ch)!=NULL))
    return 0;
  else
    return -2;
}


void FUNCTION_CALL_MODE split(std::string const& str, std::string const& delimiter, std::vector< std::string >* dest)
{
    size_t pos = 0, found = 0;

    while ( found != string::npos )
    {
        found = str.find(delimiter, pos);
        dest->push_back(std::string(str, pos, found - pos));
        pos = found + 1;
    }

}

void FUNCTION_CALL_MODE string_replace(string& s1, const string& s2, const string& s3)
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
}

void FUNCTION_CALL_MODE replaceStr(char* pBuf, int bufLen, const string& s2,const string& s3)
{
	string s1(pBuf);
	string_replace(s1, s2, s3);
	snprintf(pBuf, bufLen, "%s", s1.c_str());
}

double FUNCTION_CALL_MODE string_to_double(std::string str)
{
    char* endptr;
    double value = strtod(str.c_str(), &endptr);
    if (*endptr) return 0;
    return value;
}

int FUNCTION_CALL_MODE string_to_int(std::string str)
{
	return atoi( str.c_str());
}

int FUNCTION_CALL_MODE hs_strcmp(const char *d_str, const char *s_str)
{
    if ((d_str != NULL) && (s_str != NULL))
    {
        return strcmp(d_str, s_str);
    }
    else
    {
        return -1;
    }
}
