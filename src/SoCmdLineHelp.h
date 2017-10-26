//-----------------------------------------------------------------------------
#ifndef _SoCmdLineHelp_h_
#define _SoCmdLineHelp_h_
//--------------------------------------------------------------------
#define SoCmdLineHelp_GlobalBuffSize 2048
//-----------------------------------------------------------------------------
class SoCmdLineHelp
{
public:
	static char* GetValueByKey(const char* szCmdLine, const char* szKey);
	static int GetIntByKey(const char* szCmdLine, const char* szKey, const int nDefaultValue);
	static float GetFloatByKey(const char* szCmdLine, const char* szKey, const float fDefaultValue);
};
//-----------------------------------------------------------------------------
#endif //_SoCmdLineHelp_h_
//-----------------------------------------------------------------------------
