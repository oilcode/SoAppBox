//-----------------------------------------------------------------------------
#ifndef _SoBoxPackManager_h_
#define _SoBoxPackManager_h_
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
class SoBoxPackManager
{
public:
	static void CreateBoxPackManager();
	static void ReleaseBoxPackManager();
	static SoBoxPackManager* Get();

	//������װ����
	bool Pack();

private:
	SoBoxPackManager();
	~SoBoxPackManager();

	//����SoBoxResourceĿ¼���ռ�Ŀ¼�����е��ļ���
	bool ScanDir_SoBoxResource();
	//��UserResourceĿ¼ִ�д��������
	bool PackUserResource();
	//���UserResource.7z�ļ��Ƿ����ɣ�������ɣ�������ӵ�m_vecFileList�б��С�
	bool CheckUserResource7z();
	//����SoBox.exe��������ΪSoBox_setup.exe��
	bool MakeSoBoxSetupExe();
	//��m_vecFileList�м�¼���ļ���ӵ�SoBox_setup.exe�С�
	bool AppendFileList();

private:
	static SoBoxPackManager* ms_pInstance;
	//�洢���е���Ҫ������exe�ļ��ϵ���Դ�ļ����ļ���
	std::vector<std::string> m_vecFileList;
	//SoBox_setup.exe���ļ�ָ��
	FILE* m_pSetupFile;
};
//-----------------------------------------------------------------------------
inline SoBoxPackManager* SoBoxPackManager::Get()
{
	return ms_pInstance;
}
//-----------------------------------------------------------------------------
#endif //_SoBoxPackManager_h_
//-----------------------------------------------------------------------------
