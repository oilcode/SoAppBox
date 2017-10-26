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
	//��pInitPath��pSubPathƴ����������Ŀ��Ŀ¼���ݹ����Ŀ��Ŀ¼������Ŀ¼�µ��ļ���
	//�������ļ��󣬰Ѹ��ļ������pInitPath��·����Ҳ����·����pSubPath��ʼ�㣩�����ļ����ϲ�����ӵ�vecFiles�С�
	//--pInitPath ·����ĩβҪ����'\\'����'/'
	//--pSubPath ·����ĩβҪ����'\\'����'/'
	//--szFileNameFilter �ļ����˲�����ʹ��_findfirst����ʱ�������ù��˲�����ֵΪ"*.*"����ʾ���е��ļ�������������
	//--vecFiles [out]�����õ����ļ��б�
	static void ScanDir(const char* pInitPath, const char* pSubPath, const char* szFileNameFilter, std::vector<std::string>& vecFiles);

	//�жϴ����ļ��Ƿ���ڡ�
	static bool IsFileExist(const char* szFileName);

};
//--------------------------------------------------------------------
#endif //_SoDirectoryHelp_h_
//--------------------------------------------------------------------
