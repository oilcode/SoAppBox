//-----------------------------------------------------------------------------
//1，用户可以压入SoLua_MaxCount_PushElement个参数，然后执行Call()调用lua脚本函数，
//   得到的返回值都暂时存储在本类中，用户可以调用GetXX()得到返回值。
//   参数和返回值都必须是简单类型的变量。
//2，指定table和key，得到键值对中的value。用户可以压入SoLua_MaxCount_PushElement个key，
//   然后执行Field()，得到的value都暂时存储在本类中，用户可以调用GetXX()得到value。
//   参数和返回值都必须是简单类型的变量。
//3，目前，只能获取键值对中的value，不能设置键值对中的value。
//4，在lua绑定逻辑中，C++函数的参数是从lua脚本中传递过来的，其中字符串类型的参数一定
//   不是NULL，即使lua脚本中传递的是nil，绑定逻辑中也会把它转换成空字符串""。
//5，在lua绑定逻辑中，字符串类型的参数是浅拷贝，并没有拷贝到本类维护的缓存中，
//   字符串类型的参数指向的是lua栈中的缓存，所以不能从栈中弹出元素或者清空栈。
//   使用浅拷贝是为了提高执行效率。
//6，压入栈时，使用push系列的函数，不需要指定序号，依次逐个压入栈。
//   取出栈内元素时，使用get系列函数，需要指定序号，这样做的用意是，避免一个语法上的陷阱。
//   例如，依次取出栈内元素后，第一个元素是theWindowID，第二个元素是theWindowWidth，
//   用户希望的效果是 SetWindowWidth(theWindowID, theWindowWidth) ，
//   如果不指定序号，执行 SetWindowWidth(get(), get()) 后，用户得到的效果是
//   SetWindowWidth(theWindowWidth, theWindowID)
//   正确的写法是 SetWindowWidth(get(0), get(1))
//7，压入的值不能是NULL(nil)，因为NULL的值是0，Push(0)不能区分出是压入数字0还是压入NULL。
//--------------------------------------------------------------------
//去除编译warning
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS
//-----------------------------------------------------------------------------
#include "SoLua.h"
#include "SoSimpleLog.h"
#include "SoStringHelp.h"
#include <stdio.h>
#include <stdlib.h>
//-----------------------------------------------------------------------------
const char* SoLua::ms_kPushTableList[SoLua_MaxCount_PushTable];
SoLua::stElement SoLua::ms_kPushElementList[SoLua_MaxCount_PushElement];
SoLua::stElement SoLua::ms_kPopElementList[SoLua_MaxCount_PopElement];
char SoLua::ms_szStringValueList[SoLua_MaxStringCount][SoLua_MaxStringSize];
lua_State* SoLua::ms_L = 0;
//-----------------------------------------------------------------------------
void SoLua::InitLua()
{
	if (ms_L == 0)
	{
		ms_L = lua_open();
		luaL_openlibs(ms_L);
	}
	PushClear();
}
//-----------------------------------------------------------------------------
void SoLua::ReleaseLua()
{
	if (ms_L != 0)
	{
		lua_close(ms_L);
		ms_L = 0;
	}
}
//-----------------------------------------------------------------------------
bool SoLua::ExecuteFile(const char* pszLuaFile)
{
	if (ms_L==0 || pszLuaFile==0)
	{
		return false;
	}
	//从lua文件中获取文件内容。
	//可以是二进制内容也可以是文本内容。
	char* pFileBuff = 0;
	int nFileSize = 0;
	if (LoadFileToBuff(pszLuaFile, &pFileBuff, nFileSize) == false)
	{
		return false;
	}
	//编译并执行。
	bool br = ExecuteTrunk(pFileBuff, nFileSize);
	//释放缓存。
	if (pFileBuff)
	{
		free(pFileBuff);
	}
	return br;
}
//--------------------------------------------------------------------
bool SoLua::ExecuteTrunk(const char* pBuff, int nBuffSize)
{
	lua_State* L = ms_L;
	if (L==0 || pBuff==0)
	{
		return false;
	}
	if (nBuffSize <= 0)
	{
		nBuffSize = SoStrLen(pBuff);
	}
	int nError = luaL_loadbuffer(L, pBuff, nBuffSize, 0); //把字符串内容编译成一个函数，并压入栈
	if (nError == 0)
	{
		//加载成功
		nError = lua_pcall(L, 0, 0, 0); //执行lua函数，并把lua函数弹出栈；没有参数，也没有返回值
		if (nError == 0)
		{
			//执行成功
			//什么也不做
		}
		else
		{
			//执行失败
			const char* pszErrorMsg = lua_tostring(L, -1);
			SoLogError("SoLua::ExecuteTrunk : call fail [%s]", pszErrorMsg);
			lua_pop(L, 1); //把错误提示信息弹出栈
		}
	}
	else
	{
		//加载失败
		const char* pszErrorMsg = lua_tostring(L, -1);
		SoLogError("SoLua::ExecuteTrunk : load fail [%s]", pszErrorMsg);
		lua_pop(L, 1); //把错误提示信息弹出栈
	}
	return (nError == 0);
}
//--------------------------------------------------------------------
bool SoLua::LoadFileToBuff(const char* pszFile, char** ppBuff, int& nBuffSize)
{
	if (pszFile==0 || ppBuff==0)
	{
		return false;
	}
	//打开文件.
	FILE* fp = fopen(pszFile, "rb");
	if (fp == 0)
	{
		return false;
	}
	//计算文件大小.
	fseek(fp, 0, SEEK_END);
	nBuffSize = (int)ftell(fp);
	//分配Buff内存.
	*ppBuff = (char*)malloc(nBuffSize);
	if (*ppBuff == 0)
	{
		//申请内存失败。
		return false;
	}
	//填充Buff。
	fseek(fp, 0, SEEK_SET);
	const size_t sizeFileSize = (size_t)nBuffSize;
	size_t nActuallyRead = fread(*ppBuff, 1, sizeFileSize, fp);
	if (nActuallyRead != sizeFileSize)
	{
		free(*ppBuff);
		*ppBuff = 0;
		fclose(fp);
		return false;
	}
	//关闭文件。
	fclose(fp);
	return true;
}
//--------------------------------------------------------------------
void SoLua::DumpStack()
{
	lua_State* L = ms_L;
	const int count = lua_gettop(L);
	SoLogDebug("SoLua::DumpStack : begin; element count[%d]", count);
	//
	for (int i = 1; i <= count; ++i)
	{
		const int type = lua_type(L, i);
		switch (type)
		{
		case LUA_TSTRING:
			{
				const char* szString = lua_tostring(L, i);
				SoLogDebug("SoLua::DumpStack : [%d][string][%s]", i, szString);
				break;
			}
		case LUA_TBOOLEAN:
			{
				const char* szBool = lua_toboolean(L, i) ? "true" : "false";
				SoLogDebug("SoLua::DumpStack : [%d][bool][%s]", i, szBool);
				break;
			}
		case LUA_TNUMBER:
			{
				double dfNumber = lua_tonumber(L, i);
				SoLogDebug("SoLua::DumpStack : [%d][number][%f]", i, dfNumber);
				break;
			}
		default:
			{
				const char* szTypeName = lua_typename(L, type);
				SoLogDebug("SoLua::DumpStack : [%d][%s][]", i, szTypeName);
				break;
			}
		}
	}
	SoLogDebug("SoLua::DumpStack : end");
}
//-----------------------------------------------------------------------------
void SoLua::PushClear()
{
	for (int i = 0; i < SoLua_MaxCount_PushTable; ++i)
	{
		ms_kPushTableList[i] = 0;
	}
	for (int i = 0; i < SoLua_MaxCount_PushElement; ++i)
	{
		ms_kPushElementList[i].nType = ElementType_none;
	}
}
//--------------------------------------------------------------------
void SoLua::PushTable(const char* szTableName)
{
	if (szTableName == 0)
	{
		SoLogError("SoLua::PushTable : TableName must not be NULL");
		return;
	}
	//
	int nPushIndex = -1;
	for (int i = 0; i < SoLua_MaxCount_PushTable; ++i)
	{
		if (ms_kPushTableList[i] == 0)
		{
			nPushIndex = i;
			break;
		}
	}
	if (nPushIndex == -1)
	{
		SoLogError("SoLua::PushTable : over flow");
		return;
	}
	//
	ms_kPushTableList[nPushIndex] = szTableName;
}
//--------------------------------------------------------------------
void SoLua::Push(double dfValue)
{
	const int nPushIndex = FindEmptyPushIndex();
	if (nPushIndex == -1)
	{
		SoLogError("SoLua::Push : over flow");
		return;
	}
	ms_kPushElementList[nPushIndex].nType = ElementType_double;
	ms_kPushElementList[nPushIndex].dfValue = dfValue;
}
//--------------------------------------------------------------------
void SoLua::Push(const char* szValue)
{
	const int nPushIndex = FindEmptyPushIndex();
	if (nPushIndex == -1)
	{
		SoLogError("SoLua::Push : over flow");
		return;
	}
	ms_kPushElementList[nPushIndex].nType = ElementType_string;
	ms_kPushElementList[nPushIndex].szValue = szValue;
}
//--------------------------------------------------------------------
void SoLua::Push(bool bValue)
{
	const int nPushIndex = FindEmptyPushIndex();
	if (nPushIndex == -1)
	{
		SoLogError("SoLua::Push : over flow");
		return;
	}
	ms_kPushElementList[nPushIndex].nType = ElementType_bool;
	ms_kPushElementList[nPushIndex].bValue = bValue ? 1 : 0;
}
//-----------------------------------------------------------------------------
bool SoLua::CallEnd(const char* pszFunc)
{
	lua_State* L = ms_L;
	if (L==0 || pszFunc==0 || pszFunc[0]==0)
	{
		return false;
	}
	//记录已经压入了多少个table
	const int nTableCount = PushTableList();
	if (nTableCount == -1)
	{
		return false;
	}
	//
	if (nTableCount == 0)
	{
		lua_getglobal(L, pszFunc); //从全局环境中找到pszFunc并压入栈；如果找不到则压入nil
	}
	else
	{
		lua_getfield(L, -1, pszFunc); //从table中找到pszFunc并压入栈；如果找不到则压入nil
	}
	if (lua_isfunction(L, -1) == false)
	{
		lua_pop(L, nTableCount+1); //找不到这个function。把以前的table和刚压入的nil弹出栈
		SoLogError("SoLua::CallEnd : Can not find the function[%s]", pszFunc);
		return false;
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//压入参数
	int nArgCount = 0;
	for (int i = 0; i < SoLua_MaxCount_PushElement; ++i)
	{
		const int theType = ms_kPushElementList[i].nType;
		if (theType == ElementType_none)
		{
			break;
		}
		//
		switch (theType)
		{
			case ElementType_double:
			{
				lua_pushnumber(L, ms_kPushElementList[i].dfValue); //压入栈
				break;
			}
			case ElementType_string:
			{
				lua_pushstring(L, ms_kPushElementList[i].szValue); //压入栈
				break;
			}
			case ElementType_bool:
			{
				lua_pushboolean(L, ms_kPushElementList[i].bValue); //压入栈
				break;
			}
			default:
			{
				SoLogError("SoLua::CallEnd : invalid element type[%d]", theType);
				break;
			}
		}
		++nArgCount;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//没有办法预知有多少个返回值，所以返回值的个数总是SoLua_MaxCount_PopElement个。
	const int nResultCount = SoLua_MaxCount_PopElement;
	//执行lua函数，并把lua函数弹出栈；
	//如果执行成功则把(nResultCount)个返回值压入栈；
	//如果lua函数的实际返回值少于nResultCount个，则压入nil补全；
	//如果lua函数的实际返回值多于nResultCount个，则截断；
	//如果执行失败则把错误信息压入栈。
	if (lua_pcall(L, nArgCount, nResultCount, 0) != 0)
	{
		//执行失败.
		const char* pszErrorMsg = lua_tostring(L, -1);
		SoLogError("SoLua::CallEnd : lua_pcall fail [%s][%s]", pszFunc, pszErrorMsg);
		lua_pop(L, nTableCount+1); //把table和刚压入的错误提示信息弹出栈
		return false;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//取出返回值
	int nPopCount = 0;
	int nStringValueCount = 0; //记录返回值中字符串类型的个数
	for (int i = -nResultCount; i < 0; ++i)
	{
		//取出value
		CopyResultValue(i, nPopCount, nStringValueCount, true);
		//
		++nPopCount;
	}
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	lua_pop(L, nTableCount+nResultCount); //把table和返回值弹出栈
	return true;
}
//-----------------------------------------------------------------------------
bool SoLua::FieldEnd()
{
	//记录已经压入了多少个table
	const int nTableCount = PushTableList(); //压入所有的table
	if (nTableCount == -1)
	{
		return false;
	}
	//记录返回值中字符串类型的个数
	int nStringValueCount = 0;
	for (int i = 0; i < SoLua_MaxCount_PushElement; ++i)
	{
		if (i >= SoLua_MaxCount_PopElement)
		{
			//返回值的个数已经达到最大个数
			SoLogError("SoLua::FieldEnd : pop element over flow");
			break;
		}
		const int theType = ms_kPushElementList[i].nType;
		if (theType == ElementType_none)
		{
			//遍历key完毕
			break;
		}
		else if (theType != ElementType_string)
		{
			//key必须是string类型
			SoLogError("SoLua::FieldEnd : invalid element type[%d]", theType);
			break;
		}
		//
		if (nTableCount == 0)
		{
			lua_getglobal(ms_L, ms_kPushElementList[i].szValue); //先压入key，找到value，然后弹出key并压入value；如果找不到则压入nil
		}
		else
		{
			lua_getfield(ms_L, -1, ms_kPushElementList[i].szValue); //先压入key，找到value，然后弹出key并压入value；如果找不到则压入nil
		}
		//取出value
		CopyResultValue(-1, i, nStringValueCount, true);
		//
		lua_pop(ms_L, 1); //把value弹出栈
	}
	//
	lua_pop(ms_L, nTableCount); //把所有的table弹出栈
	return true;
}
//-----------------------------------------------------------------------------
double SoLua::GetDouble(int nIndex, double dfDefault)
{
	double dfValue = dfDefault;
	if (IsValidPopIndex(nIndex))
	{
		if (ms_kPopElementList[nIndex].nType == ElementType_double)
		{
			dfValue = ms_kPopElementList[nIndex].dfValue;
		}
	}
	return dfValue;
}
//-----------------------------------------------------------------------------
const char* SoLua::GetString(int nIndex, const char* szDefault)
{
	const char* szValue = szDefault;
	if (IsValidPopIndex(nIndex))
	{
		if (ms_kPopElementList[nIndex].nType == ElementType_string)
		{
			szValue = ms_kPopElementList[nIndex].szValue;
		}
	}
	return szValue;
}
//-----------------------------------------------------------------------------
bool SoLua::GetBool(int nIndex, bool bDefault)
{
	bool bValue = bDefault;
	if (IsValidPopIndex(nIndex))
	{
		if (ms_kPopElementList[nIndex].nType == ElementType_bool)
		{
			bValue = (ms_kPopElementList[nIndex].bValue != 0);
		}
	}
	return bValue;
}
//-----------------------------------------------------------------------------
int SoLua::PushTableList()
{
	//记录已经压入了多少个table
	int nTableCount = 0;
	for (int i = 0; i < SoLua_MaxCount_PushTable; ++i)
	{
		const char* szTableName = ms_kPushTableList[i];
		if (szTableName != 0)
		{
			if (nTableCount == 0)
			{
				lua_getglobal(ms_L, szTableName); //从全局环境中找到szTableName并压入栈；如果找不到则压入nil
			}
			else
			{
				lua_getfield(ms_L, -1, szTableName); //从前一个table中找到szTableName并压入栈；如果找不到则压入nil
			}
			if (lua_istable(ms_L, -1) == true)
			{
				++nTableCount;
			}
			else
			{
				lua_pop(ms_L, nTableCount+1); //找不到szTableName，把以前的table和刚压入的nil弹出栈
				SoLogError("SoLua::PushTableList : Can not find the table[%s]", szTableName);
				return -1;
			}
		}
		else
		{
			//遍历结束
			break;
		}
	}
	return nTableCount;
}
//-----------------------------------------------------------------------------
bool SoLua::CopyResultValue(const int nStackIndex, const int nPopElementIndex, int& nStringValueCount, bool bDeepCopy)
{
	bool br = true;
	const int luatype = lua_type(ms_L, nStackIndex);
	switch (luatype)
	{
	case LUA_TNUMBER:
		{
			double dfV = lua_tonumber(ms_L, nStackIndex);
			ms_kPopElementList[nPopElementIndex].nType = ElementType_double;
			ms_kPopElementList[nPopElementIndex].dfValue = dfV;
			break;
		}
	case LUA_TSTRING:
		{
			const char* szValue = lua_tostring(ms_L, nStackIndex);
			ms_kPopElementList[nPopElementIndex].nType = ElementType_string;
			ms_kPopElementList[nPopElementIndex].szValue = szValue; //浅拷贝
			if (bDeepCopy) //深拷贝
			{
				if (nStringValueCount < SoLua_MaxStringCount)
				{
					SoStrCpy(ms_szStringValueList[nStringValueCount], SoLua_MaxStringSize, szValue);
					ms_szStringValueList[nStringValueCount][SoLua_MaxStringSize-1] = 0;
					ms_kPopElementList[nPopElementIndex].szValue = ms_szStringValueList[nStringValueCount];
					++nStringValueCount;
				}
				else
				{
					ms_kPopElementList[nPopElementIndex].szValue = "";
					br = false;
					SoLogError("SoLua::CopyResultValue : output string value count too much");
				}
			}
			break;
		}
	case LUA_TBOOLEAN:
		{
			int bV = lua_toboolean(ms_L, nStackIndex);
			ms_kPopElementList[nPopElementIndex].nType = ElementType_bool;
			ms_kPopElementList[nPopElementIndex].bValue = bV;
			break;
		}
	case LUA_TNIL:
		{
			//脚本逻辑中可能会返回nil，这是正常情况，也要占用一个stElement。
			ms_kPopElementList[nPopElementIndex].nType = ElementType_none;
			break;
		}
	default:
		{
			ms_kPopElementList[nPopElementIndex].nType = ElementType_none;
			br = false;
			//
			const char* szTypeName = lua_typename(ms_L, luatype);
			SoLogError("SoLua::CopyResultValue : invalid output value type[%s]", szTypeName);
			break;
		}
	}
	return br;
}
//-----------------------------------------------------------------------------
