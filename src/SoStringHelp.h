//--------------------------------------------------------------------
//�����ڿ�ƽ̨���������
#ifndef _SoStringHelp_h_
#define _SoStringHelp_h_
//--------------------------------------------------------------------
#define SoStringHelp_GlobalBuffSize 2048
//--------------------------------------------------------------------
//�����ַ������ȡ�
//�ַ������Ȳ��ܴ���0x3FFFFFFF��1G�ࣩ��
//�����������ֵ��Ŀ���ǣ���ֹ�û�������һ��û����0��β���ַ�����������ѭ����
int SoStrLen(const char* szString);

//�ַ���������
//�����ַ������ȡ�
int SoStrCpy(char* DestBuff, const int nDestBuffSize, const char* szSourceString);

//�ַ�����ʽ����ʹ�ñ�ģ���ṩ���ַ������档
//����ֵ����const char*���ǿ����޸ĵ��ַ������棬�����������Զ��ַ�����һЩ�޸ġ�
char* SoStrFmt(const char* szFormat, ...);
//�ַ�����ʽ����ʹ��ָ�����ַ�������
void SoStrFmtSelf(char* Buff, const int nBuffSize, const char* szFormat, ...);
char* SoStrFmtByVaList(const char* szFormat, const void* pValist);
void SoStrFmtSelfByVaList(char* Buff, const int nBuffSize, const char* szFormat, const void* pValist);

//�Ƚ������ַ���
int SoStrCmp(const char* s1, const char* s2);
//�Ƚ������ַ����������Ǵ�Сд����
int SoStrCmpNoCase(const char* s1, const char* s2);

//�����ַ�c���ַ���str���״γ��ֵ�λ����š�
//�����Դ����ַ���str�ĳ��ȣ����������Ļ��������ڲ�������ַ������ȡ�
//����ҵĵ�������ֵ��Χ[0, SoStrLen(str))��
//����Ҳ���������ֵΪ-1��
int SoStrChr(const char* str, const char c, int nStrLen = -1);
//�����ַ�c���ַ���str��ĩ�γ��ֵ�λ����š�
//�����Դ����ַ���str�ĳ��ȣ����������Ļ��������ڲ�������ַ������ȡ�
//����ҵĵ�������ֵ��Χ[0, SoStrLen(str))��
//����Ҳ���������ֵΪ-1��
int SoStrRChr(const char* str, const char c, int nStrLen = -1);

//���ַ����е��ַ��滻�ɴ�д��ĸ
char* SoStrToUpper(const char* szString);
void SoStrToUpperSelf(char* szString);

//���ַ����е��ַ��滻��Сд��ĸ
char* SoStrToLower(const char* szString);
void SoStrToLowerSelf(char* szString);

//·���ַ�����׼��
//�Ѵ�����ַ����޸ĳ����¸�ʽ��
//1����'\\'�޸ĳ�'/'��
//2���Ѵ�д��ĸ�޸ĳ�Сд��ĸ��
//����ֵ��һ��ȫ�ֵ��ַ������飬���Ҫ���콫����ֵȡ�ߡ�
char* SoPathNormalize(const char* pszString);
void SoPathNormalizeSelf(char* szString);

// Same to ::atof, but strip the string, remain 7 numbers after '.' before call atof.
// Why we need this? Because in android c++_static, atof ( and std::atof ) is unsupported for numbers
// have long decimal part and contain several numbers can approximate to 1 ( like 90.099998474121094 ), 
// it will return inf. This function is used to fix this bug.
double Soatof(const char* szString);
// Same to atoi
int Soatoi(const char* szString);
//--------------------------------------------------------------------
#endif //_SoStringHelp_h_
//--------------------------------------------------------------------
