//-----------------------------------------------------------------------------
#ifndef _SoBoxHelp_h_
#define _SoBoxHelp_h_
//-----------------------------------------------------------------------------
#include <string>
#include <cstdio>
//-----------------------------------------------------------------------------
enum eOperationType
{
	OP_None, //无效值
	OP_Pack, //打包，制作安装包
	OP_Unpack, //解压包，把安装包携带的资源释放出来
	OP_Max,
};
//-----------------------------------------------------------------------------
#define SoBoxSetup_ResourceFileNameSize 256
//-----------------------------------------------------------------------------
class SoBoxHelp
{
public:
	//根据exe的完整路径解析出工作路径。
	static bool ParseWorkPath(const char* szExePath);
	static const char* GetWorkPath();

	//解析命令行参数。
	static bool ParseCmdLine(const char* szCmdLine);
	static eOperationType GetOperationType();
	static bool GetLogEnable();
	
	//把pSrcFile文件的内容拷贝到pDestFile文件中。
	static bool FileCopy(FILE* pDestFile, FILE* pSrcFile);

private:
	//存储工作路径。工作路径的末尾含有斜杠分隔符。
	//ms_strWorkPath + "SoBox.exe" 就是exe的完整路径。
	static std::string ms_strWorkPath;
	//操作类型。
	static eOperationType ms_eOperationType;
	//是否输出log。
	static bool ms_bLogEnable;
};
//-----------------------------------------------------------------------------
#endif //_SoBoxHelp_h_
//-----------------------------------------------------------------------------
