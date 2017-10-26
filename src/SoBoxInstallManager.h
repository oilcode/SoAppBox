//-----------------------------------------------------------------------------
#ifndef _SoBoxInstallManager_h_
#define _SoBoxInstallManager_h_
//-----------------------------------------------------------------------------
class SoBoxInstallManager
{
public:
	static void CreateBoxInstallManager();
	static void ReleaseBoxInstallManager();
	static SoBoxInstallManager* Get();

	//执行安装过程。
	bool Install();

private:
	SoBoxInstallManager();
	~SoBoxInstallManager();

private:
	static SoBoxInstallManager* ms_pInstance;

};
//-----------------------------------------------------------------------------
inline SoBoxInstallManager* SoBoxInstallManager::Get()
{
	return ms_pInstance;
}
//-----------------------------------------------------------------------------
#endif //_SoBoxInstallManager_h_
//-----------------------------------------------------------------------------
