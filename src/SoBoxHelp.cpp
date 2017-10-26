//-----------------------------------------------------------------------------
#include "SoBoxHelp.h"
#include "SoStringHelp.h"
#include "SoCmdLineHelp.h"
//-----------------------------------------------------------------------------
std::string SoBoxHelp::ms_strWorkPath;
eOperationType SoBoxHelp::ms_eOperationType = OP_None;
bool SoBoxHelp::ms_bLogEnable = false;
//-----------------------------------------------------------------------------
bool SoBoxHelp::ParseWorkPath(const char* szExePath)
{
	if (szExePath == 0 || szExePath[0] == 0)
	{
		return false;
	}
	const int nLen = SoStrLen(szExePath);
	const int nPos = SoStrRChr(szExePath, '\\', nLen);
	if (nPos == -1)
	{
		return false;
	}

	ms_strWorkPath = szExePath;
	ms_strWorkPath[nPos + 1] = 0;
	return true;
}
//-----------------------------------------------------------------------------
const char* SoBoxHelp::GetWorkPath()
{
	return ms_strWorkPath.c_str();
}
//-----------------------------------------------------------------------------
bool SoBoxHelp::ParseCmdLine(const char* szCmdLine)
{
	if (szCmdLine == 0 || szCmdLine[0] == 0)
	{
		return false;
	}
	//获取操作类型
	const int nOpValue = SoCmdLineHelp::GetIntByKey(szCmdLine, "op", ms_eOperationType);
	if (nOpValue > OP_None && nOpValue < OP_Max)
	{
		ms_eOperationType = (eOperationType)nOpValue;
	}
	else
	{
		return false;
	}
	//获取是否开启log
	const int nLogValue = SoCmdLineHelp::GetIntByKey(szCmdLine, "log", ms_bLogEnable ? 1 : 0);
	ms_bLogEnable = nLogValue > 0 ? true : false;
	//获取工作路径。
	//如果命令行中携带了工作路径，则优先使用这里的路径。
	//当SoBox.exe是由其他程序拉起的时候，其他程序就需要把工作路径传递给SoBox.exe。
	char* szWorkPath = SoCmdLineHelp::GetValueByKey(szCmdLine, "path");
	if (szWorkPath && szWorkPath[0] != 0)
	{
		int nLen = SoStrLen(szWorkPath);
		if (szWorkPath[nLen - 2] != '/' && szWorkPath[nLen - 2] != '\\')
		{
			//保证工作路径的末尾含有斜杠分隔符。
			szWorkPath[nLen - 1] = '/';
			szWorkPath[nLen] = 0;
			++nLen;
		}
		ms_strWorkPath = szWorkPath;
	}

	return true;
}
//-----------------------------------------------------------------------------
eOperationType SoBoxHelp::GetOperationType()
{
	return ms_eOperationType;
}
//-----------------------------------------------------------------------------
bool SoBoxHelp::GetLogEnable()
{
	return ms_bLogEnable;
}
//-----------------------------------------------------------------------------
bool SoBoxHelp::FileCopy(FILE* pDestFile, FILE* pSrcFile)
{
	if (pDestFile == 0 || pSrcFile == 0)
	{
		return false;
	}

	const int nBuffSize = 2048;
	char* buff = (char*)malloc(nBuffSize);
	if (buff == 0)
	{
		return false;
	}

	while (feof(pSrcFile) == 0)
	{
		int nReadSize = (int)fread(buff, 1, nBuffSize, pSrcFile);
		fwrite(buff, 1, nReadSize, pDestFile);
	}

	fflush(pDestFile);
	free(buff);
	return true;
}
//-----------------------------------------------------------------------------
