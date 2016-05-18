#include <Windows.h>
#include "TPtrArray.h"

class TString
{
public:
	void UpdateLength();
	wchar_t* Pointer();
	wchar_t* Clone();
	char* ToChar(const wchar_t*,int*);
	wchar_t* ToWideChar(const char*,int*);
	void Set(const wchar_t*);
	const char* GetAsChar();
	const wchar_t* GetAsWChar();
	const wchar_t* GetStringAt(long);
	wchar_t* GetOwner();
	wchar_t GetAt(int);
	bool IsWm(const wchar_t*);
	bool IsBegin(const wchar_t*);
	bool IsBegin(const wchar_t*, const wchar_t *);
	bool IsEQ(const wchar_t*);
	bool IsEQ(const wchar_t*,const wchar_t*);
	bool IsEQNC(const wchar_t*);
	static int Cmp(const wchar_t*, const wchar_t*);
	bool IsNum();
	const wchar_t* SubStr(unsigned long nStart,unsigned long nLength);
	void ToLower();
	void ToUpper();
	void Replace(const wchar_t*,const wchar_t*);
	void Trim();
	void Split(const wchar_t*,TPtrArray*);
	long Pos(const wchar_t*,int);
	const wchar_t* Add(const char*);
	const wchar_t* Add(const wchar_t*);
	const wchar_t* AddNL();
	wchar_t* GetString();
	long GetLength();
	TString(const char*);
	TString(const wchar_t*);
	TString(long);
	TString();
	virtual ~TString();
	bool operator ==(wchar_t*);
	void operator =(const char*);
	void operator =(const wchar_t*);
	void operator +=(const char*);
	void operator +=(const wchar_t*);
	const wchar_t* operator +(const char*);
	const wchar_t* operator +(const wchar_t*);
	void operator +=(const char);
	void operator +=(const wchar_t);
	void operator +=(long);
	void operator +=(int);
	wchar_t operator [](long);

protected:
	bool IsBegin(const char*,const char*);
	unsigned long m_lLength;
	long m_lPntStr;
	long m_lPntStrC;
};
