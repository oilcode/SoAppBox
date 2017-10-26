//-----------------------------------------------------------------------------
#include "SoCmdLineHelp.h"
#include "SoStringHelp.h"
//-----------------------------------------------------------------------------
//公共缓存区，用于存储返回值
char g_Buff[SoCmdLineHelp_GlobalBuffSize] = { 0 };
//键与值之间的间隔符号有哪些
//空格，等号，冒号
const char* g_Signal = " =:";
//键值对结束的标识符有哪些
//空格，逗号，分号
const char* g_EndFlag = " ,;{}";
//-----------------------------------------------------------------------------
bool CheckIsSignal(const char theChar)
{
	bool br = false;
	int nIndex = 0;
	while (g_Signal[nIndex] != 0)
	{
		if (g_Signal[nIndex] == theChar)
		{
			br = true;
			break;
		}
		else
		{
			++nIndex;
		}
	}
	return br;
}
//-----------------------------------------------------------------------------
bool CheckIsEndFlag(const char theChar)
{
	bool br = false;
	int nIndex = 0;
	while (g_EndFlag[nIndex] != 0)
	{
		if (g_EndFlag[nIndex] == theChar)
		{
			br = true;
			break;
		}
		else
		{
			++nIndex;
		}
	}
	return br;
}
//-----------------------------------------------------------------------------
char* SoCmdLineHelp::GetValueByKey(const char* szCmdLine, const char* szKey)
{
	//清空公共缓存区
	g_Buff[0] = 0;

	if (szCmdLine == 0 || szCmdLine[0] == 0)
	{
		return g_Buff;
	}
	if (szKey == 0 || szKey[0] == 0)
	{
		return g_Buff;
	}

	const int nKeyLen = SoStrLen(szKey);

	//第一步，在szCmdLine中查找key所在的位置
	int nCurIndex = 0;
	bool bFindKey = false;
	//记录前一个字符是否为“键值对结束的标识符”
	bool bLastKeyIsEndFlag = true;
	while (szCmdLine[nCurIndex] != 0)
	{
		const bool bCurKeyIsEndFlag = CheckIsEndFlag(szCmdLine[nCurIndex]);
		if (bLastKeyIsEndFlag == true && bCurKeyIsEndFlag == false)
		{
			//“键值对结束的标识符”的后面遇到非结束符，则开始判断是否为Key字符串
			if (szCmdLine[nCurIndex] == szKey[0])
			{
				//命令行中有个字符与Key的第一个字符相同，判断Key字符串是否出现
				bool bAppear = true;
				for (int i = 0; i < nKeyLen; ++i)
				{
					if (szCmdLine[nCurIndex+i] == szKey[i])
					{
						//相同，继续比较，什么都不做
					}
					else
					{
						//不相同
						bAppear = false;
						break;
					}
				}
				if (bAppear == true)
				{
					//Key字符串出现了，判断接下来的字符是否为“键与值之间的间隔符号”，
					//如果不是“键与值之间的间隔符号”，说明并没有找到真正的Key字符串。
					if (CheckIsSignal(szCmdLine[nCurIndex+nKeyLen]) == true)
					{
						bFindKey = true;
					}
				}
			}
		}
		//
		if (bFindKey)
		{
			//此时，szCmdLine[nCurIndex]就是szKey[0]
			break;
		}
		else
		{
			bLastKeyIsEndFlag = bCurKeyIsEndFlag;
			++nCurIndex;
		}
	}

	if (bFindKey == false)
	{
		//命令行中没有找到Key字符串
		return g_Buff;
	}

	//已经找到Key字符串
	//让下标序号跳过Key字符串
	nCurIndex += nKeyLen;
	//此时szCmdLine[nCurIndex]就是“键与值之间的间隔符号”
	//跳过间隔符号和空格，找到值字符串的第一个字符
	bool bFindValue = false;
	while (szCmdLine[nCurIndex] != 0)
	{
		if (CheckIsSignal(szCmdLine[nCurIndex]) == true)
		{
			++nCurIndex;
		}
		else
		{
			bFindValue = true;
			break;
		}
	}
	if (bFindValue == false)
	{
		//没有找到Value字符串
		return g_Buff;
	}

	//把Value字符串拷贝到g_Buff中，直到遇到“键值对结束的标识符”
	int nValueLen = 0;
	while (szCmdLine[nCurIndex] != 0)
	{
		if (CheckIsEndFlag(szCmdLine[nCurIndex]) == true)
		{
			g_Buff[nValueLen] = 0;
			break;
		}
		else
		{
			g_Buff[nValueLen] = szCmdLine[nCurIndex];
			++nValueLen;
			++nCurIndex;
		}
	}

	//再次设置结束符。
	//如果while循环的结束不是从break跳出，而是遇到了szCmdLine[nCurIndex]值为0，
	//则g_Buff不是以0结尾的。所以需要再次设置结束符。
	g_Buff[nValueLen] = 0;
	return g_Buff;
}
//-----------------------------------------------------------------------------
int SoCmdLineHelp::GetIntByKey(const char* szCmdLine, const char* szKey, const int nDefaultValue)
{
	const char* szValue = GetValueByKey(szCmdLine, szKey);
	if (szValue[0] != 0)
	{
		return Soatoi(szValue);
	}
	else
	{
		return nDefaultValue;
	}
}
//-----------------------------------------------------------------------------
float SoCmdLineHelp::GetFloatByKey(const char* szCmdLine, const char* szKey, const float fDefaultValue)
{
	const char* szValue = GetValueByKey(szCmdLine, szKey);
	if (szValue[0] != 0)
	{
		return (float)Soatof(szValue);
	}
	else
	{
		return fDefaultValue;
	}
}
//-----------------------------------------------------------------------------
