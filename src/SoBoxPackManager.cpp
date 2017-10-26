//--------------------------------------------------------------------
//去除编译warning
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
#include "SoBoxPackManager.h"
#include "SoBoxHelp.h"
#include "SoDirectoryHelp.h"
#include "SoSimpleLog.h"
#include "SoBatProcess.h"
#include "SoStringHelp.h"
//-----------------------------------------------------------------------------
SoBoxPackManager* SoBoxPackManager::ms_pInstance = 0;
//-----------------------------------------------------------------------------
void SoBoxPackManager::CreateBoxPackManager()
{
	if (ms_pInstance == 0)
	{
		ms_pInstance = new SoBoxPackManager;
	}
}
//-----------------------------------------------------------------------------
void SoBoxPackManager::ReleaseBoxPackManager()
{
	if (ms_pInstance)
	{
		delete ms_pInstance;
		ms_pInstance = 0;
	}
}
//-----------------------------------------------------------------------------
SoBoxPackManager::SoBoxPackManager()
:m_pSetupFile(0)
{

}
//-----------------------------------------------------------------------------
SoBoxPackManager::~SoBoxPackManager()
{

}
//-----------------------------------------------------------------------------
bool SoBoxPackManager::Pack()
{
	bool br = true;
	do 
	{
		if (ScanDir_SoBoxResource() == false)
		{
			br = false;
			break;
		}
		if (PackUserResource() == false)
		{
			br = false;
			break;
		}
		if (CheckUserResource7z() == false)
		{
			br = false;
			break;
		}
		if (MakeSoBoxSetupExe() == false)
		{
			br = false;
			break;
		}
		if (AppendFileList() == false)
		{
			br = false;
			break;
		}
	} while (1);

	if (m_pSetupFile)
	{
		fclose(m_pSetupFile);
		m_pSetupFile = 0;
	}
	return br;
}
//-----------------------------------------------------------------------------
bool SoBoxPackManager::ScanDir_SoBoxResource()
{
	SoDirectoryHelp::ScanDir(SoBoxHelp::GetWorkPath(), "SoBoxResource/", "*.*", m_vecFileList);
	const int nSize = (int)m_vecFileList.size();

	if (SoBoxHelp::GetLogEnable())
	{
		SoLogDebug("ScanDir_SoBoxResource : Size[%d]", nSize);
		for (int i = 0; i < nSize; ++i)
		{
			SoLogDebug("[%s]", m_vecFileList[i].c_str());
		}
	}

	if (nSize > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//-----------------------------------------------------------------------------
bool SoBoxPackManager::PackUserResource()
{
	std::string strPackCmdFile = SoBoxHelp::GetWorkPath();
	strPackCmdFile += "SoBoxResource\\Pack.bat";
	strPackCmdFile += " ";
	strPackCmdFile += SoBoxHelp::GetWorkPath();
	bool br = SoBatProcess::StartBat(strPackCmdFile.c_str());
	if (br)
	{
		SoBatProcess::WaitForFinish();
	}
	return br;
}
//-----------------------------------------------------------------------------
bool SoBoxPackManager::CheckUserResource7z()
{
	std::string strFilePath = SoBoxHelp::GetWorkPath();
	std::string strFileName = "userresource.7z";
	strFilePath += strFileName;
	bool bFileExist = SoDirectoryHelp::IsFileExist(strFilePath.c_str());
	if (bFileExist)
	{
		m_vecFileList.push_back(strFileName);
	}
	return bFileExist;
}
//-----------------------------------------------------------------------------
bool SoBoxPackManager::MakeSoBoxSetupExe()
{
	std::string strSrcExeName = SoBoxHelp::GetWorkPath();
	strSrcExeName += "SoBox.exe";
	std::string strDestExeName = SoBoxHelp::GetWorkPath();
	strDestExeName += "SoBox_setup.exe";

	FILE* pSrcExe = fopen(strSrcExeName.c_str(), "rb");
	if (pSrcExe == 0)
	{
		SoLogError("MakeSoBoxSetupExe : open file failed [%s]", strSrcExeName.c_str());
		return false;
	}
	m_pSetupFile = fopen(strDestExeName.c_str(), "wb+");
	if (m_pSetupFile == 0)
	{
		SoLogError("MakeSoBoxSetupExe : create file failed [%s]", strDestExeName.c_str());
		return false;
	}

	bool bCopyResult = SoBoxHelp::FileCopy(m_pSetupFile, pSrcExe);
	fclose(pSrcExe);
	//不要关闭m_pSetupFile文件指针。
	return bCopyResult;
}
//-----------------------------------------------------------------------------
bool SoBoxPackManager::AppendFileList()
{
	bool bResult = true;
	char* FileNameBuff = (char*)malloc(SoBoxSetup_ResourceFileNameSize);
	const int nCount = (int)m_vecFileList.size();
	for (int i = 0; i < nCount; ++i)
	{
		std::string strFile = SoBoxHelp::GetWorkPath();
		strFile += m_vecFileList[i];
		//打开文件
		FILE* pSrcFile = fopen(strFile.c_str(), "rb");
		if (pSrcFile == 0)
		{
			bResult = false;
			break;
		}
		//添加文件
		bool bCopyResult = SoBoxHelp::FileCopy(m_pSetupFile, pSrcFile);
		if (bCopyResult == false)
		{
			bResult = false;
			break;
		}
		//添加文件名字
		memset(FileNameBuff, 0, SoBoxSetup_ResourceFileNameSize);
		SoStrCpy(FileNameBuff, SoBoxSetup_ResourceFileNameSize, m_vecFileList[i].c_str());
		fwrite(FileNameBuff, 1, SoBoxSetup_ResourceFileNameSize, m_pSetupFile);
		//添加文件大小
		fseek(pSrcFile, 0, SEEK_END);
		const int nFileSize = (int)ftell(pSrcFile);
		fwrite(&nFileSize, sizeof(int), 1, m_pSetupFile);
		//
		fclose(pSrcFile);
		fflush(m_pSetupFile);
	}
	if (bResult == true)
	{
		//把资源文件的个数添加到末尾
		fwrite(&nCount, sizeof(int), 1, m_pSetupFile);
	}
	//
	free(FileNameBuff);
	return bResult;
}
//-----------------------------------------------------------------------------
