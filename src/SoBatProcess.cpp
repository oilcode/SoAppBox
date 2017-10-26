//--------------------------------------------------------------------
#include "SoBatProcess.h"
#include <process.h>
//--------------------------------------------------------------------
HANDLE SoBatProcess::ms_pThreadHandle = NULL;
std::string SoBatProcess::ms_strBatFile;
//--------------------------------------------------------------------
void SoBatProcess::InitBatProcess()
{
	//ʲô��������
}
//--------------------------------------------------------------------
void SoBatProcess::ReleaseBatProcess()
{
	WaitForFinish();
}
//--------------------------------------------------------------------
bool SoBatProcess::StartBat(const char* szBatFile)
{
	if (szBatFile == 0 || szBatFile[0] == 0)
	{
		return false;
	}
	if (IsRunning())
	{
		return false;
	}
	//�����������̡߳�
	ms_pThreadHandle = (HANDLE)_beginthreadex(NULL, 0, SoBatProcess::ThreadProcess, NULL, CREATE_SUSPENDED, NULL);
	if (ms_pThreadHandle == NULL)
	{
		return false;
	}
	//�����̡߳�
	ms_strBatFile = szBatFile;
	ResumeThread(ms_pThreadHandle);
	//
	return true;
}
//--------------------------------------------------------------------
bool SoBatProcess::IsRunning()
{
	return (ms_pThreadHandle != NULL);
}
//--------------------------------------------------------------------
void SoBatProcess::WaitForFinish()
{
	if (ms_pThreadHandle)
	{
		//�ȴ����߳̽���
		WaitForSingleObject(ms_pThreadHandle, INFINITE);
		ms_pThreadHandle = NULL;
	}
}
//--------------------------------------------------------------------
unsigned int WINAPI SoBatProcess::ThreadProcess(void* p)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	if (CreateProcessA(NULL,
		(LPSTR)ms_strBatFile.c_str(),
		NULL,
		NULL,
		FALSE,
		0, //CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	OnThreadFinished();
	return 0;
}
//--------------------------------------------------------------------
void SoBatProcess::OnThreadFinished()
{
	if (ms_pThreadHandle)
	{
		CloseHandle(ms_pThreadHandle);
		ms_pThreadHandle = NULL;
	}
	ms_strBatFile.clear();
}
//--------------------------------------------------------------------
