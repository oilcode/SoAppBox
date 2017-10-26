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

	//制作安装包。
	bool Pack();

private:
	SoBoxPackManager();
	~SoBoxPackManager();

	//遍历SoBoxResource目录，收集目录下所有的文件。
	bool ScanDir_SoBoxResource();
	//对UserResource目录执行打包操作。
	bool PackUserResource();
	//检查UserResource.7z文件是否生成，如果生成，把它添加到m_vecFileList列表中。
	bool CheckUserResource7z();
	//拷贝SoBox.exe并重命名为SoBox_setup.exe。
	bool MakeSoBoxSetupExe();
	//把m_vecFileList中记录的文件添加到SoBox_setup.exe中。
	bool AppendFileList();

private:
	static SoBoxPackManager* ms_pInstance;
	//存储所有的需要附加在exe文件上的资源文件的文件名
	std::vector<std::string> m_vecFileList;
	//SoBox_setup.exe的文件指针
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
