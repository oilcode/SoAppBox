//--------------------------------------------------------------------
#ifndef _SoBatProcess_h_
#define _SoBatProcess_h_
//--------------------------------------------------------------------
#include <string>
#include <Windows.h>
//--------------------------------------------------------------------
class SoBatProcess
{
public:
	//初始化
	static void InitBatProcess();
	//释放本类维护的资源，当主进程结束时调用此函数。
	static void ReleaseBatProcess();

	//开始执行一个bat文件。
	static bool StartBat(const char* szBatFile);
	//判断是否正在执行bat文件。
	static bool IsRunning();
	//等待bat文件执行完毕。
	static void WaitForFinish();

private:
	//子线程使用的函数
	static unsigned int WINAPI ThreadProcess(void* p);
	//响应子线程结束的函数
	static void OnThreadFinished();

private:
	static HANDLE ms_pThreadHandle;
	static std::string ms_strBatFile;
};
//--------------------------------------------------------------------
#endif //_SoBatProcess_h_
//--------------------------------------------------------------------
