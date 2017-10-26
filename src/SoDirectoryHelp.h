//--------------------------------------------------------------------
#ifndef _SoDirectoryHelp_h_
#define _SoDirectoryHelp_h_
//--------------------------------------------------------------------
#include <string>
#include <vector>
//--------------------------------------------------------------------
class SoDirectoryHelp
{
public:
	//把pInitPath和pSubPath拼接起来就是目标目录，递归遍历目标目录及其子目录下的文件，
	//搜索到文件后，把该文件相对于pInitPath的路径（也即，路径从pSubPath开始算）及其文件名合并后，添加到vecFiles中。
	//--pInitPath 路径的末尾要含有'\\'或者'/'
	//--pSubPath 路径的末尾要含有'\\'或者'/'
	//--szFileNameFilter 文件过滤参数，使用_findfirst函数时可以设置过滤参数。值为"*.*"，表示所有的文件都被搜索到。
	//--vecFiles [out]遍历得到的文件列表
	static void ScanDir(const char* pInitPath, const char* pSubPath, const char* szFileNameFilter, std::vector<std::string>& vecFiles);

	//判断磁盘文件是否存在。
	static bool IsFileExist(const char* szFileName);

};
//--------------------------------------------------------------------
#endif //_SoDirectoryHelp_h_
//--------------------------------------------------------------------
