//-----------------------------------------------------------------------------
//1，不支持Unicode字符集。
//2，每次创建本类的对象，都会在与exe同目录下新建一个log文件。
//   该log文件名的末尾会加上时间戳，确保每次都是新的文件。
//-----------------------------------------------------------------------------
#ifndef _SoSimpleLog_h_
#define _SoSimpleLog_h_
//-----------------------------------------------------------------------------
class SoSimpleLog
{
public:
	static void CreateSimpleLog();
	static void ReleaseSimpleLog();
	static SoSimpleLog* Get();

	void OutputDebug(const char* pFormat, ...);
	void OutputWarning(const char* pFormat, ...);
	void OutputError(const char* pFormat, ...);

private:
	SoSimpleLog();
	~SoSimpleLog();
	//初始化函数。
	bool InitLog();
	void AddLogHead(char* pType);
	void AddLogBody(const char* pFormat, const void* pVaList);
	//在与exe同目录下新建一个log文件。
	//该log文件名的末尾会加上时间戳，确保每次都是新的文件。
	bool OpenDiskFile();
	//关闭磁盘文件。
	void CloseDiskFile();

private:
	static SoSimpleLog* ms_pInstance;
private:
	void* m_fp;
	bool m_bOutputDebugString;
	bool m_bFlushImmediately;
};
//-----------------------------------------------------------------------------
inline SoSimpleLog* SoSimpleLog::Get()
{
	return ms_pInstance;
}
//-----------------------------------------------------------------------------
#define SoLogDebug if(SoSimpleLog::Get()) SoSimpleLog::Get()->OutputDebug
#define SoLogWarning if(SoSimpleLog::Get()) SoSimpleLog::Get()->OutputWarning
#define SoLogError if(SoSimpleLog::Get()) SoSimpleLog::Get()->OutputError
//-----------------------------------------------------------------------------
#endif //_SoSimpleLog_h_
//-----------------------------------------------------------------------------
