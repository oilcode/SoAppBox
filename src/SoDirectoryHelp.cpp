//--------------------------------------------------------------------
//ȥ������warning
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS
//--------------------------------------------------------------------
#include "SoDirectoryHelp.h"
#include "SoStringHelp.h"
#include <io.h> //Ϊ�˵���_findfirstϵ�к���
#include <cstdio>
//--------------------------------------------------------------------
void SoDirectoryHelp::ScanDir(const char* pInitPath, const char* pSubPath, const char* szFileNameFilter, std::vector<std::string>& vecFiles)
{
	//������ǰĿ¼�µ����е��ļ�
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

	//������ǰĿ¼�µ����е���Ŀ¼������ÿ����Ŀ¼ִ�б�������
	std::string strSearchSubFolder = pInitPath;
	strSearchSubFolder += pSubPath;
	strSearchSubFolder += "*.*"; //ע�⣬����������Ŀ¼������ʹ��szFileNameFilter��
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
				//�ݹ����
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
