//-----------------------------------------------------------------------
#include "SoBoxHelp.h"
#include "SoSimpleLog.h"
#include "SoBatProcess.h"
#include "SoLua.h"
#include "SoDirectoryHelp.h"
#include "SoBoxPackManager.h"
#include <string>
//-----------------------------------------------------------------------
int main(int argc, char* argv[])
{
	int nResult = 0;
	//----------------------------------------
	const char* szExePath = argv[0];
	const char* szCmdLine = 0;
	if (argc >= 2)
	{
		szCmdLine = argv[1];
	}
	//----------------------------------------
	do
	{
		//----------------------------------------
		if (SoBoxHelp::ParseCmdLine(szCmdLine) == false)
		{
			nResult = 1;
			break;
		}
		//----------------------------------------
		if (SoBoxHelp::GetLogEnable())
		{
			SoSimpleLog::CreateSimpleLog();
			SoLogDebug("SoBox Start");
			SoLogDebug("CmdLine [%s]", szCmdLine);
		}
		//----------------------------------------
		if (SoBoxHelp::ParseWorkPath(szExePath) == false)
		{
			nResult = 2;
			break;
		}
		SoLogDebug("WorkPath [%s]", SoBoxHelp::GetWorkPath());
		//----------------------------------------
		SoBatProcess::InitBatProcess();
		SoLua::InitLua();
		//----------------------------------------
		if (SoBoxHelp::GetOperationType() == OP_Pack)
		{
			SoBoxPackManager::CreateBoxPackManager();
			if (SoBoxPackManager::Get())
			{
				SoBoxPackManager::Get()->Pack();
			}
			SoBoxPackManager::ReleaseBoxPackManager();
		}
		else if (SoBoxHelp::GetOperationType() == OP_Unpack)
		{
			std::string strUnpackCmdFile = SoBoxHelp::GetWorkPath();
			strUnpackCmdFile += "SoBoxResource\\Unpack.bat";
			strUnpackCmdFile += " ";
			strUnpackCmdFile += SoBoxHelp::GetWorkPath();
			SoBatProcess::StartBat(strUnpackCmdFile.c_str());
		}

		//----------------------------------------
	} while (0);
	//----------------------------------------

	
	SoLua::ReleaseLua();
	SoBatProcess::ReleaseBatProcess();
	SoLogDebug("SoBox Finish");
	SoSimpleLog::ReleaseSimpleLog();
	return nResult;
}
//-----------------------------------------------------------------------
