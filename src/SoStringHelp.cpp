//--------------------------------------------------------------------
//致力于跨平台，跨编译器
//--------------------------------------------------------------------
//去除编译warning
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS
//--------------------------------------------------------------------
#include "SoStringHelp.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
//--------------------------------------------------------------------
//本模块使用的缓存
char g_szBuff[SoStringHelp_GlobalBuffSize] = {0};
//--------------------------------------------------------------------
int SoStrLen(const char* szString)
{
	int nLen = 0;
	for (nLen = 0; nLen < 0x3FFFFFFF; ++nLen)
	{
		if (*szString == 0)
		{
			break;
		}
		else
		{
			++szString;
		}
	}
	return nLen;
}
//--------------------------------------------------------------------
int SoStrCpy(char* DestBuff, const int nDestBuffSize, const char* szSourceString)
{
	int nLen = 0;
	const int nMaxLen = nDestBuffSize - 1;
	for (nLen = 0; nLen < nMaxLen; ++nLen)
	{
		if (*szSourceString == 0)
		{
			break;
		}
		else
		{
			*DestBuff = *szSourceString;
			++DestBuff;
			++szSourceString;
		}
	}
	*DestBuff = 0;
	return nLen;
}
//--------------------------------------------------------------------
char* SoStrFmt(const char* szFormat, ...)
{
	va_list klist;
	va_start(klist, szFormat);
	vsnprintf(g_szBuff, SoStringHelp_GlobalBuffSize, szFormat, klist);
	va_end(klist);
	return g_szBuff;
}
//--------------------------------------------------------------------
void SoStrFmtSelf(char* Buff, const int nBuffSize, const char* szFormat, ...)
{
	va_list klist;
	va_start(klist, szFormat);
	vsnprintf(Buff, nBuffSize, szFormat, klist);
	va_end(klist);
}
//--------------------------------------------------------------------
char* SoStrFmtByVaList(const char* szFormat, const void* pValist)
{
	va_list* pList = (va_list*)pValist;
	vsnprintf(g_szBuff, SoStringHelp_GlobalBuffSize, szFormat, *pList);
	return g_szBuff;
}
//--------------------------------------------------------------------
void SoStrFmtSelfByVaList(char* Buff, const int nBuffSize, const char* szFormat, const void* pValist)
{
	va_list* pList = (va_list*)pValist;
	vsnprintf(Buff, nBuffSize, szFormat, *pList);
}
//--------------------------------------------------------------------
int SoStrCmp(const char* s1, const char* s2)
{
	const unsigned char* us1 = (const unsigned char*)s1;
	const unsigned char* us2 = (const unsigned char*)s2;

	while (*us1 == *us2)
	{
		if (*us1 == 0)
		{
			return 0;
		}
		else
		{
			++us1;
			++us2;
		}
	}
	return (*us1 - *us2);
}
//--------------------------------------------------------------------
int SoStrCmpNoCase(const char* s1, const char* s2)
{
	const unsigned char* us1 = (const unsigned char*)s1;
	const unsigned char* us2 = (const unsigned char*)s2;
	unsigned char tolower1 = 1;
	unsigned char tolower2 = 1;

	while (tolower1 == tolower2)
	{
		if (tolower1 == 0)
		{
			return 0;
		}
		else
		{
			tolower1 = *us1++;
			tolower2 = *us2++;
			if (tolower1 >= 65 && tolower1 <= 90)
			{
				tolower1 += 32;
			}
			if (tolower2 >= 65 && tolower2 <= 90)
			{
				tolower2 += 32;
			}
		}
	}
	return (tolower1 - tolower2);
}
//--------------------------------------------------------------------
int SoStrChr(const char* str, const char c, int nStrLen)
{
	int nPos = -1;
	if (nStrLen == -1)
	{
		nStrLen = SoStrLen(str);
	}
	//
	for (int i = 0; i < nStrLen; ++i)
	{
		if (*str == c)
		{
			nPos = i;
			break;
		}
		else
		{
			++str;
		}
	}
	return nPos;
}
//--------------------------------------------------------------------
int SoStrRChr(const char* str, const char c, int nStrLen)
{
	int nPos = -1;
	if (nStrLen == -1)
	{
		nStrLen = SoStrLen(str);
	}
	//
	const char* r_str = str + nStrLen - 1;
	for (int i = nStrLen - 1; i >= 0; --i)
	{
		if (*r_str == c)
		{
			nPos = i;
			break;
		}
		else
		{
			--r_str;
		}
	}
	return nPos;
}
//--------------------------------------------------------------------
char* SoStrToUpper(const char* szString)
{
	int nCount = 0;
	while (szString[nCount] != 0)
	{
		char theC = szString[nCount];
		if (theC >= 'a' && theC <= 'z')
		{
			theC -= 32;
		}
		g_szBuff[nCount] = theC;
		//
		++nCount;
		if (nCount >= SoStringHelp_GlobalBuffSize)
		{
			g_szBuff[SoStringHelp_GlobalBuffSize-1] = 0;
			break;
		}
	}
	if (nCount < SoStringHelp_GlobalBuffSize)
	{
		g_szBuff[nCount] = 0;
	}
	return g_szBuff;
}
//--------------------------------------------------------------------
void SoStrToUpperSelf(char* szString)
{
	int nCount = 0;
	while (szString[nCount] != 0)
	{
		char& theC = szString[nCount];
		if (theC >= 'a' && theC <= 'z')
		{
			theC -= 32;
		}
		//
		++nCount;
	}
}
//--------------------------------------------------------------------
char* SoStrToLower(const char* szString)
{
	int nCount = 0;
	while (szString[nCount] != 0)
	{
		char theC = szString[nCount];
		if (theC >= 'A' && theC <= 'Z')
		{
			theC += 32;
		}
		g_szBuff[nCount] = theC;
		//
		++nCount;
		if (nCount >= SoStringHelp_GlobalBuffSize)
		{
			g_szBuff[SoStringHelp_GlobalBuffSize-1] = 0;
			break;
		}
	}
	if (nCount < SoStringHelp_GlobalBuffSize)
	{
		g_szBuff[nCount] = 0;
	}
	return g_szBuff;
}
//--------------------------------------------------------------------
void SoStrToLowerSelf(char* szString)
{
	int nCount = 0;
	while (szString[nCount] != 0)
	{
		char& theC = szString[nCount];
		if (theC >= 'A' && theC <= 'Z')
		{
			theC += 32;
		}
		//
		++nCount;
	}
}
//--------------------------------------------------------------------
char* SoPathNormalize(const char* pszString)
{
	int nCount = 0;
	while (pszString[nCount] != 0)
	{
		char theC = pszString[nCount];
		if (theC >= 'A' && theC <= 'Z')
		{
			//大写字母转换为小写字母
			theC += 32;
		}
		else if (theC == '\\')
		{
			theC = '/';
		}
		g_szBuff[nCount] = theC;
		//
		++nCount;
		if (nCount >= SoStringHelp_GlobalBuffSize)
		{
			g_szBuff[SoStringHelp_GlobalBuffSize-1] = 0;
			break;
		}
	}
	if (nCount < SoStringHelp_GlobalBuffSize)
	{
		g_szBuff[nCount] = 0;
	}
	return g_szBuff;
}
//--------------------------------------------------------------------
void SoPathNormalizeSelf(char* szString)
{
	int nCount = 0;
	while (szString[nCount] != 0)
	{
		char& theC = szString[nCount];
		if (theC >= 'A' && theC <= 'Z')
		{
			//大写字母转换为小写字母
			theC += 32;
		}
		else if (theC == '\\')
		{
			theC = '/';
		}
		//
		++nCount;
	}
}
//--------------------------------------------------------------------
double Soatof(const char* szString)
{
	if (szString == 0)
	{
		return 0.0;
	}

	const int nStrLen = SoStrCpy(g_szBuff, SoStringHelp_GlobalBuffSize, szString);
	// strip string, only remain 7 numbers after '.'
	int nPos = SoStrChr(g_szBuff, '.', nStrLen);
	if (nPos != -1 && nPos + 8 < SoStringHelp_GlobalBuffSize)
	{
		g_szBuff[nPos + 8] = 0;
	}

	return atof(g_szBuff);
}
//--------------------------------------------------------------------
int Soatoi(const char* szString)
{
	if (szString == 0)
	{
		return 0;
	}
	else
	{
		return atoi(szString);
	}
}
//--------------------------------------------------------------------
