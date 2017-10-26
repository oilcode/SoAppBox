//--------------------------------------------------------------------
//去除编译warning
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS
//--------------------------------------------------------------------
#include "SoDirectoryHelp.h"
#include "SoStringHelp.h"
#include <io.h> //为了调用_findfirst系列函数
#include <cstdio>
//--------------------------------------------------------------------
void SoDirectoryHelp::ScanDir(const char* pInitPath, const char* pSubPath, const char* szFileNameFilter, std::vector<std::string>& vecFiles)
{
	//遍历当前目录下的所有的文件
	std::string strSubPath = pSubPath;
	std::string strSearchTarget = pInitPath;
	strSearchTarget += pSubPath;
	strSearchTarget += szFileNameFilter;
	struct _finddata_t fileInfo;
	//
	long handle = (long)_findfirst(strSearchTarget.c_str(), &fileInfo);
	if (handle != -1)
	{
		while (1)
		{
			if (!(fileInfo.attrib & _A_SUBDIR))
			{
				std::string strSubPathFile = strSubPath + fileInfo.name;
				strSubPathFile = SoPathNormalize(strSubPathFile.c_str());
				vecFiles.push_back(strSubPathFile);
			}
			//
			if (_findnext(handle, &fileInfo) == -1)
			{
				break;
			}
		}
		//
		_findclose(handle);
	}

	//遍历当前目录下的所有的子目录，并对每个子目录执行遍历操作
	std::string strSearchSubFolder = pInitPath;
	strSearchSubFolder += pSubPath;
	strSearchSubFolder += "*.*"; //注意，这里是搜索目录，不能使用szFileNameFilter。
	handle = (long)_findfirst(strSearchSubFolder.c_str(), &fileInfo);
	if (handle != -1)
	{
		while (1)
		{
			if (fileInfo.attrib & _A_SUBDIR && fileInfo.name[0] != '.')
			{
				std::string strNewSubPath = strSubPath;
				strNewSubPath += fileInfo.name;
				strNewSubPath += '/';
				//递归调用
				ScanDir(pInitPath, strNewSubPath.c_str(), szFileNameFilter, vecFiles);
			}
			//
			if (_findnext(handle, &fileInfo) == -1)
			{
				break;
			}
		}
		//
		_findclose(handle);
	}
}
//--------------------------------------------------------------------
bool SoDirectoryHelp::IsFileExist(const char* szFileName)
{
	bool bExist = false;
	FILE* fp = fopen(szFileName, "rb");
	if (fp)
	{
		fclose(fp);
		bExist = true;
	}
	return bExist;
}
//--------------------------------------------------------------------
