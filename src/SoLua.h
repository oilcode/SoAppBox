//-----------------------------------------------------------------------------
#ifndef _SoLua_h_
#define _SoLua_h_
//-----------------------------------------------------------------------------
extern "C"
{
#include "lualib/lua.h"
#include "lualib/lualib.h"
#include "lualib/lauxlib.h"
}
//-----------------------------------------------------------------------------
//每次执行lua栈操作时，最多压入多少个table
#define SoLua_MaxCount_PushTable 5
//每次执行lua栈操作时，最多压入多少个参数
#define SoLua_MaxCount_PushElement 10
//每次执行lua栈操作时，最多有多少个返回值
#define SoLua_MaxCount_PopElement 10
//lua函数的返回值允许是字符串类型的值。
//该字符串会拷贝到本类维护的buff内，供外界使用。
//本类维护的buff会重复利用，外界要尽快取走buff内的字符串。
//本类维护多少个buff，也即lua函数最多有几个字符串类型的返回值。
#define SoLua_MaxStringCount 10
//每个buff内有多少个字节。
#define SoLua_MaxStringSize 1024
//-----------------------------------------------------------------------------
class SoLua
{
public:
	//初始化lua环境。
	static void InitLua();
	//释放lua环境。
	static void ReleaseLua();
	//获取lua环境。
	static lua_State* GetLua();

	//载入并执行一个lua文件。
	static bool ExecuteFile(const char* pszLuaFile);
	//执行一个代码块。可以是文本代码块，也可以是二进制代码块。
	//如果nBuffSize值为0，则认为pBuff是一个带结束符的字符串，本函数内部会计算字符串长度。
	static bool ExecuteTrunk(const char* pBuff, int nBuffSize);
	//把当前的lua栈打印出来。
	static void DumpStack();

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//把所有的函数参数清零。
	static void PushClear();
	//依次压入table名字。
	static void PushTable(const char* szTableName); //注意，字符串szTableName是浅拷贝。
	//依次压入lua函数的参数。
	static void Push(double dfValue);
	static void Push(float fValue);
	static void Push(unsigned int uiValue);
	static void Push(int nValue);
	static void Push(const char* szValue); //注意，字符串szValue是浅拷贝。
	static void Push(bool bValue);
	//取出返回值。序号从0开始。
	static double GetDouble(int nIndex, double dfDefault);
	static float GetFloat(int nIndex, float fDefault);
	static unsigned int GetUInt(int nIndex, unsigned int uiDefault);
	static int GetInt(int nIndex, int nDefault);
	static const char* GetString(int nIndex, const char* szDefault);
	static bool GetBool(int nIndex, bool bDefault);
	//执行一个（table内的）lua函数。
	static void CallBegin();
	static bool CallEnd(const char* pszFunc);
	//获取若干个（table内的）键值对中的value。
	static void FieldBegin();
	static bool FieldEnd();
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

private:
	//从磁盘上打开指定的文件，把文件内容读取到指定的Buff内。
	//函数内部会为Buff分配内存，由外界负责删除Buff。
	static bool LoadFileToBuff(const char* pszFile, char** ppBuff, int& nBuffSize);
	//依次压入table名字，返回压入了多少个table。
	//返回-1表示失败了。
	static int PushTableList();
	//把栈中的第nStackIndex个元素拷贝到本类中的第nPopElementIndex个ms_kPopElementList元素。
	//--bDeepCopy 是否为深拷贝。
	static bool CopyResultValue(const int nStackIndex, const int nPopElementIndex, int& nStringValueCount, bool bDeepCopy);
	//压入函数参数时，获取存储该参数的stElement的序号。
	//返回-1表示没有找到空闲的stElement。
	static int FindEmptyPushIndex();
	//判断是否为有效的ms_kPopElementList数组下标。
	static bool IsValidPopIndex(int nIndex);

private:
	enum eElementType
	{
		ElementType_none,
		ElementType_double,
		ElementType_string,
		ElementType_bool,
	};

	struct stElement
	{
		double dfValue;
		const char* szValue;
		int bValue;
		eElementType nType;
		//
		stElement():nType(ElementType_none) {};
	};
private:
	//依次存储table名字
	static const char* ms_kPushTableList[SoLua_MaxCount_PushTable];
	//依次存储参数
	static stElement ms_kPushElementList[SoLua_MaxCount_PushElement];
	//依次存储返回值
	static stElement ms_kPopElementList[SoLua_MaxCount_PopElement];
	//存储返回值中的字符串类型的返回值
	static char ms_szStringValueList[SoLua_MaxStringCount][SoLua_MaxStringSize];
	//lua环境
	static lua_State* ms_L;
};
//-----------------------------------------------------------------------------
inline lua_State* SoLua::GetLua()
{
	return ms_L;
}
//--------------------------------------------------------------------
inline void SoLua::Push(float fValue)
{
	Push((double)fValue);
}
//--------------------------------------------------------------------
inline void SoLua::Push(unsigned int uiValue)
{
	Push((double)uiValue);
}
//--------------------------------------------------------------------
inline void SoLua::Push(int nValue)
{
	Push((double)nValue);
}
//-----------------------------------------------------------------------------
inline float SoLua::GetFloat(int nIndex, float fDefault)
{
	return (float)(GetDouble(nIndex, (double)fDefault));
}
//-----------------------------------------------------------------------------
inline unsigned int SoLua::GetUInt(int nIndex, unsigned int uiDefault)
{
	return (unsigned int)(GetDouble(nIndex, (double)uiDefault));
}
//-----------------------------------------------------------------------------
inline int SoLua::GetInt(int nIndex, int nDefault)
{
	return (int)(GetDouble(nIndex, (double)nDefault));
}
//--------------------------------------------------------------------
inline void SoLua::CallBegin()
{
	PushClear();
}
//--------------------------------------------------------------------
inline void SoLua::FieldBegin()
{
	PushClear();
}
//-----------------------------------------------------------------------------
inline int SoLua::FindEmptyPushIndex()
{
	int nPushIndex = -1;
	for (int i = 0; i < SoLua_MaxCount_PushElement; ++i)
	{
		if (ms_kPushElementList[i].nType == ElementType_none)
		{
			nPushIndex = i;
			break;
		}
	}
	return nPushIndex;
}
//-----------------------------------------------------------------------------
inline bool SoLua::IsValidPopIndex(int nIndex)
{
	return (nIndex >= 0 && nIndex < SoLua_MaxCount_PopElement);
}
//-----------------------------------------------------------------------------
#endif //_SoLua_h_
//-----------------------------------------------------------------------------
