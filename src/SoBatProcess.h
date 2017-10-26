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
	//��ʼ��
	static void InitBatProcess();
	//�ͷű���ά������Դ���������̽���ʱ���ô˺�����
	static void ReleaseBatProcess();

	//��ʼִ��һ��bat�ļ���
	static bool StartBat(const char* szBatFile);
	//�ж��Ƿ�����ִ��bat�ļ���
	static bool IsRunning();
	//�ȴ�bat�ļ�ִ����ϡ�
	static void WaitForFinish();

private:
	//���߳�ʹ�õĺ���
	static unsigned int WINAPI ThreadProcess(void* p);
	//��Ӧ���߳̽����ĺ���
	static void OnThreadFinished();

private:
	static HANDLE ms_pThreadHandle;
	static std::string ms_strBatFile;
};
//--------------------------------------------------------------------
#endif //_SoBatProcess_h_
//--------------------------------------------------------------------
