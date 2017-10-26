//-----------------------------------------------------------------------------
#ifndef _SoBoxHelp_h_
#define _SoBoxHelp_h_
//-----------------------------------------------------------------------------
#include <string>
#include <cstdio>
//-----------------------------------------------------------------------------
enum eOperationType
{
	OP_None, //��Чֵ
	OP_Pack, //�����������װ��
	OP_Unpack, //��ѹ�����Ѱ�װ��Я������Դ�ͷų���
	OP_Max,
};
//-----------------------------------------------------------------------------
#define SoBoxSetup_ResourceFileNameSize 256
//-----------------------------------------------------------------------------
class SoBoxHelp
{
public:
	//����exe������·������������·����
	static bool ParseWorkPath(const char* szExePath);
	static const char* GetWorkPath();

	//���������в�����
	static bool ParseCmdLine(const char* szCmdLine);
	static eOperationType GetOperationType();
	static bool GetLogEnable();
	
	//��pSrcFile�ļ������ݿ�����pDestFile�ļ��С�
	static bool FileCopy(FILE* pDestFile, FILE* pSrcFile);

private:
	//�洢����·��������·����ĩβ����б�ָܷ�����
	//ms_strWorkPath + "SoBox.exe" ����exe������·����
	static std::string ms_strWorkPath;
	//�������͡�
	static eOperationType ms_eOperationType;
	//�Ƿ����log��
	static bool ms_bLogEnable;
};
//-----------------------------------------------------------------------------
#endif //_SoBoxHelp_h_
//-----------------------------------------------------------------------------
