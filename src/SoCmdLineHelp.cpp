//-----------------------------------------------------------------------------
#include "SoCmdLineHelp.h"
#include "SoStringHelp.h"
//-----------------------------------------------------------------------------
//���������������ڴ洢����ֵ
char g_Buff[SoCmdLineHelp_GlobalBuffSize] = { 0 };
//����ֵ֮��ļ����������Щ
//�ո񣬵Ⱥţ�ð��
const char* g_Signal = " =:";
//��ֵ�Խ����ı�ʶ������Щ
//�ո񣬶��ţ��ֺ�
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
	//��չ���������
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

	//��һ������szCmdLine�в���key���ڵ�λ��
	int nCurIndex = 0;
	bool bFindKey = false;
	//��¼ǰһ���ַ��Ƿ�Ϊ����ֵ�Խ����ı�ʶ����
	bool bLastKeyIsEndFlag = true;
	while (szCmdLine[nCurIndex] != 0)
	{
		const bool bCurKeyIsEndFlag = CheckIsEndFlag(szCmdLine[nCurIndex]);
		if (bLastKeyIsEndFlag == true && bCurKeyIsEndFlag == false)
		{
			//����ֵ�Խ����ı�ʶ�����ĺ��������ǽ���������ʼ�ж��Ƿ�ΪKey�ַ���
			if (szCmdLine[nCurIndex] == szKey[0])
			{
				//���������и��ַ���Key�ĵ�һ���ַ���ͬ���ж�Key�ַ����Ƿ����
				bool bAppear = true;
				for (int i = 0; i < nKeyLen; ++i)
				{
					if (szCmdLine[nCurIndex+i] == szKey[i])
					{
						//��ͬ�������Ƚϣ�ʲô������
					}
					else
					{
						//����ͬ
						bAppear = false;
						break;
					}
				}
				if (bAppear == true)
				{
					//Key�ַ��������ˣ��жϽ��������ַ��Ƿ�Ϊ������ֵ֮��ļ�����š���
					//������ǡ�����ֵ֮��ļ�����š���˵����û���ҵ�������Key�ַ�����
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
			//��ʱ��szCmdLine[nCurIndex]����szKey[0]
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
		//��������û���ҵ�Key�ַ���
		return g_Buff;
	}

	//�Ѿ��ҵ�Key�ַ���
	//���±��������Key�ַ���
	nCurIndex += nKeyLen;
	//��ʱszCmdLine[nCurIndex]���ǡ�����ֵ֮��ļ�����š�
	//����������źͿո��ҵ�ֵ�ַ����ĵ�һ���ַ�
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
		//û���ҵ�Value�ַ���
		return g_Buff;
	}

	//��Value�ַ���������g_Buff�У�ֱ����������ֵ�Խ����ı�ʶ����
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

	//�ٴ����ý�������
	//���whileѭ���Ľ������Ǵ�break����������������szCmdLine[nCurIndex]ֵΪ0��
	//��g_Buff������0��β�ġ�������Ҫ�ٴ����ý�������
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
