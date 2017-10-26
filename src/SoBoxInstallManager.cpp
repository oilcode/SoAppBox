//-----------------------------------------------------------------------------
#include "SoBoxInstallManager.h"
//-----------------------------------------------------------------------------
SoBoxInstallManager* SoBoxInstallManager::ms_pInstance = 0;
//-----------------------------------------------------------------------------
void SoBoxInstallManager::CreateBoxInstallManager()
{
	if (ms_pInstance == 0)
	{
		ms_pInstance = new SoBoxInstallManager;
	}
}
//-----------------------------------------------------------------------------
void SoBoxInstallManager::ReleaseBoxInstallManager()
{
	if (ms_pInstance)
	{
		delete ms_pInstance;
		ms_pInstance = 0;
	}
}
//-----------------------------------------------------------------------------
SoBoxInstallManager::SoBoxInstallManager()
{

}
//-----------------------------------------------------------------------------
SoBoxInstallManager::~SoBoxInstallManager()
{

}
//-----------------------------------------------------------------------------
bool SoBoxInstallManager::Install()
{
	return true;
}
//-----------------------------------------------------------------------------
