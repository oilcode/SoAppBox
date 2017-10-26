//--------------------------------------------------------------------
//去除编译warning
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
#include "SoSimpleLog.h"
#include "SoTimeHelp.h"
#include "SoStringHelp.h"
#include <cstdio> //磁盘文件操作
#include <Windows.h> //为了使用不定参数va_list，和函数OutputDebugString
//-----------------------------------------------------------------------------
SoSimpleLog* SoSimpleLog::ms_pInstance = 0;
//-----------------------------------------------------------------------------
void SoSimpleLog::CreateSimpleLog()
{
	if (ms_pInstance == 0)
	{
		ms_pInstance = new SoSimpleLog;
		if (ms_pInstance)
		{
			if (ms_pInstance->InitLog() == false)
			{
				delete ms_pInstance;
				ms_pInstance = 0;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void SoSimpleLog::ReleaseSimpleLog()
{
	if (ms_pInstance)
	{
		delete ms_pInstance;
		ms_pInstance = 0;
	}
}
//-----------------------------------------------------------------------------
SoSimpleLog::SoSimpleLog()
:m_fp(0)
,m_bOutputDebugString(true)
,m_bFlushImmediately(true)
{

}
//-----------------------------------------------------------------------------
SoSimpleLog::~SoSimpleLog()
{
	CloseDiskFile();
}
//-----------------------------------------------------------------------------
bool SoSimpleLog::InitLog()
{
	m_bOutputDebugString = true;
	m_bFlushImmediately = true;
	bool br = OpenDiskFile();
	return br;
}
//-----------------------------------------------------------------------------
void SoSimpleLog::OutputDebug(const char* pFormat, ...)
{
	if (pFormat == 0)
	{
		return;
	}
	//
	AddLogHead("DEBUG");
	//
	va_list kList;
	va_start(kList, pFormat);
	AddLogBody(pFormat, &kList);
	va_end(kList);
}
//-----------------------------------------------------------------------------
void SoSimpleLog::OutputWarning(const char* pFormat, ...)
{
	if (pFormat == 0)
	{
		return;
	}
	//
	AddLogHead("WARNING");
	//
	va_list kList;
	va_start(kList, pFormat);
	AddLogBody(pFormat, &kList);
	va_end(kList);
}
//-----------------------------------------------------------------------------
void SoSimpleLog::OutputError(const char* pFormat, ...)
{
	if (pFormat == 0)
	{
		return;
	}
	//
	AddLogHead("ERROR");
	//
	va_list kList;
	va_start(kList, pFormat);
	AddLogBody(pFormat, &kList);
	va_end(kList);
}
//-----------------------------------------------------------------------------
void SoSimpleLog::AddLogHead(char* pType)
{
	stTimeTM _tm;
	SoTimeHelp::GetLocalTimeTM_Millisecond(&_tm);
	const char* szHead = SoStrFmt("%02u:%02u:%02u:%03u [%s] ", _tm.nHour, _tm.nMinute, _tm.nSecond, _tm.nMillisecond, pType);
	if (m_fp)
	{
		int nLen = SoStrLen(szHead);
		fwrite(szHead, 1, nLen, (FILE*)m_fp);
	}
	if (m_bOutputDebugString)
	{
		OutputDebugStringA(szHead);
	}
}
//-----------------------------------------------------------------------------
void SoSimpleLog::AddLogBody(const char* pFormat, const void* pVaList)
{
	char* szBuff = SoStrFmtByVaList(pFormat, pVaList);
	if (m_fp)
	{
		int nLen = SoStrLen(szBuff);
		if (nLen > SoStringHelp_GlobalBuffSize - 2)
		{
			nLen = SoStringHelp_GlobalBuffSize - 2;
		}
		//添加换行符
		szBuff[nLen] = '\n';
		++nLen;
		szBuff[nLen] = 0;
		//
		fwrite(szBuff, 1, nLen, (FILE*)m_fp);
		if (m_bFlushImmediately)
		{
			fflush((FILE*)m_fp);
		}
	}
	if (m_bOutputDebugString)
	{
		OutputDebugStringA(szBuff);
	}
}
//-----------------------------------------------------------------------------
bool SoSimpleLog::OpenDiskFile()
{
	//获取当前时间
	stTimeTM _tm;
	SoTimeHelp::GetLocalTimeTM(&_tm);

	//组装log文件名字
	const char* szFileName = SoStrFmt("Log_%04u%02u%02u-%02u%02u%02u.txt", _tm.nYear, _tm.nMonth, _tm.nDayInMonth, _tm.nHour, _tm.nMinute, _tm.nSecond);

	//打开文件
	m_fp = fopen(szFileName, "at+");
	if (m_fp == 0)
	{
		//打开文件失败。
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
void SoSimpleLog::CloseDiskFile()
{
	if (m_fp)
	{
		fflush((FILE*)m_fp);
		fclose((FILE*)m_fp);
		m_fp = 0;
	}
}
//-----------------------------------------------------------------------------
