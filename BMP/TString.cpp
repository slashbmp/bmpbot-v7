#include "stdafx.h"
#include "TString.h"

TString::TString()
{
	char* chr = new char[1];
	chr[0] = '\0';
	m_lPntStrC = (long)chr;
	wchar_t* wchr = new wchar_t[1];
	wchr[0] = L'\0';
	m_lPntStr = (long)wchr;
	m_lLength = 0;
}

TString::~TString()
{
	delete (char*)m_lPntStrC;
	delete (wchar_t*)m_lPntStr;
}

TString::TString(const char *str)
{
	int l;
	wchar_t* strMe = ToWideChar(str,&l);
	m_lLength = l;
	m_lPntStr = (long)strMe;
	char* chr = new char;
	chr[0] = '\0';
	m_lPntStrC = (long)chr;
}

TString::TString(const wchar_t* str) {
	m_lLength = wcslen(str);
	wchar_t* chr = new wchar_t[m_lLength + 1];
	memcpy(chr,str,sizeof(wchar_t) * m_lLength);
	chr[m_lLength] = L'\0';
	m_lPntStr = (long)chr;
	char* chrr = new char;
	chrr[0] = '\0';
	m_lPntStrC = (long)chrr;
}

TString::TString(long nLength) {
	m_lLength = 0;
	wchar_t* chr = new wchar_t[nLength + 1];
	chr[0] = L'\0';
	m_lPntStr = (long)chr;

	char* chrr = new char[1];
	chrr[0] = '\0';
	m_lPntStrC = (long)chrr;
}

long TString::GetLength()
{
	return m_lLength;
}

wchar_t* TString::GetString()
{
	return (wchar_t*)m_lPntStr;
}

bool TString::operator ==(wchar_t *str) {
	return IsEQ(str);
}

void TString::operator =(const char *str)
{
	int l;
	wchar_t* strMe = ToWideChar(str,&l);
	delete (wchar_t*)m_lPntStr;
	m_lLength = l;
	m_lPntStr = (long)strMe;
}

void TString::operator =(const wchar_t *str)
{
	delete (wchar_t*)m_lPntStr;
	m_lLength = wcslen(str);
	wchar_t* chr = new wchar_t[m_lLength + 1];
	memcpy(chr,str,sizeof(wchar_t) * m_lLength);
	chr[m_lLength] = L'\0';
	m_lPntStr = (long)chr;
}

char* TString::ToChar(const wchar_t* str,int* size = NULL)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	char* resultstring = new char[utf8size];
	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, str, -1, &resultstring[0], utf8size, NULL, NULL);
	resultstring[utf8size - 1] = '\0';
	if (size != NULL) *size = utf8size - 1;
	return resultstring;
}

wchar_t* TString::ToWideChar(const char* str,int* size = NULL)
{
	int sized = ::MultiByteToWideChar(CP_UTF8,0,str,-1,NULL,0);
	wchar_t* resultstring = new wchar_t[sized];
	::MultiByteToWideChar(CP_UTF8,0,str,-1,resultstring,sized - 1);
	resultstring[sized - 1] = L'\0';
	if (size != NULL) *size = sized - 1;
	return resultstring;
}

const wchar_t* TString::GetStringAt(long index) {
	if (index > m_lLength) index = m_lLength;
	wchar_t* str = (wchar_t*)m_lPntStr;
	return &str[index];
}

void TString::Set(const wchar_t *str)
{
	delete (wchar_t*)m_lPntStr;
	m_lLength = wcslen(str);
	wchar_t* chr = new wchar_t[m_lLength + 1];
	memcpy(chr,str,sizeof(wchar_t) * m_lLength);
	chr[m_lLength] = L'\0';
	m_lPntStr = (long)chr;
}

void TString::operator +=(const char *str) {
	Add(str);
}

void TString::operator +=(const wchar_t *str)
{
	Add(str);
}

void TString::operator +=(const char c) {
	char* cc = new char[2];
	cc[0] = c;
	cc[1] = '\0';
	Add(cc);
	delete cc;
}

void TString::operator +=(const wchar_t c) {
	wchar_t* wc = new wchar_t[2];
	wc[0] = c;
	wc[1] = L'\0';
	Add(wc);
	delete wc;
}

void TString::operator +=(long n) {
	wchar_t strNow[50];
	_ltow(n, strNow, 10);
	Add(strNow);
}

void TString::operator +=(int n) {
	wchar_t strNow[50];
	_itow(n, strNow, 10);
	Add(strNow);
}

wchar_t TString::operator [](long n) {
	if (n >= m_lLength) return L'\0';
	return ((wchar_t*)m_lPntStr)[n];
}

const wchar_t* TString::Add(const wchar_t *str)
{
	long nNewLength = wcslen(str);
	wchar_t* chr = new wchar_t[m_lLength + nNewLength + 1];
	if (m_lLength > 0) memcpy(chr,(void*)m_lPntStr,sizeof(wchar_t) * m_lLength);
	memcpy(&chr[m_lLength],str,sizeof(wchar_t) * nNewLength);
	chr[m_lLength + nNewLength] = L'\0';
	m_lLength += nNewLength;
	delete (wchar_t*)m_lPntStr;
	m_lPntStr = (long)chr;
	return (const wchar_t*)m_lPntStr;
}

const wchar_t* TString::AddNL() {
	char strNL[4];
	strNL[0] = 0x0D;
	strNL[1] = 0x00;
	strNL[2] = 0x0A;
	strNL[3] = 0x00;
	wchar_t* chr = new wchar_t[m_lLength + 5];
	if (m_lLength > 0) memcpy(chr, (void*)m_lPntStr, sizeof(wchar_t) * m_lLength);
	memcpy(&chr[m_lLength], strNL, 4);
	chr[m_lLength + 2] = L'\0';
	m_lLength += 2;
	delete (wchar_t*)m_lPntStr;
	m_lPntStr = (long)chr;
	return (const wchar_t*)m_lPntStr;
}

const wchar_t* TString::Add(const char *str)
{
	wchar_t* strMe = ToWideChar(str);
	Add(strMe);
	delete strMe;
	return (const wchar_t*)m_lPntStr;
}

const wchar_t* TString::operator +(const char *str)
{
	return Add(str);
}

const wchar_t* TString::operator +(const wchar_t *str)
{
	return Add(str);
}

const char* TString::GetAsChar()
{
	delete (char*)m_lPntStrC;
	char* strOut = ToChar((wchar_t*)m_lPntStr);
	m_lPntStrC = (long)strOut;
	return strOut;
}

const wchar_t* TString::GetAsWChar() {
	return (wchar_t*)m_lPntStr;
}

long TString::Pos(const wchar_t *str, int n)
{
	//char* str = string for search
	//int* n = 1 is position 1, 2 is position 2, 0 is all count.
	long nAll = 0;
	wchar_t* strAll = (wchar_t*)m_lPntStr;
	for (register long i = 0;strAll[i] != L'\0';i++) {
		if (strAll[i] == str[0]) {
			if (IsBegin(str,&strAll[i])) {
				nAll++;
				if (n > 0 && n == nAll) return i;
			}
		}
	}
	if (n == 0) return nAll;
	return -1;
}

bool TString::IsBegin(const wchar_t *strBegin, const wchar_t *strAll)
{
	for (register int i = 0;strBegin[i] != L'\0';i++) {
		if (strBegin[i] != strAll[i]) return false;
	}
	return true;
}

bool TString::IsBegin(const wchar_t *strBegin)
{
	wchar_t* strAll = (wchar_t*)m_lPntStr;
	for (register int i = 0;strBegin[i] != L'\0';i++) {
		if (strBegin[i] != strAll[i]) return false;
	}
	return true;
}

void TString::Split(const wchar_t *strSplit, TPtrArray *arr)
{
	long nNowPos = 0;
	long length;
	int nShift = wcslen(strSplit);
	wchar_t* strAll = (wchar_t*)m_lPntStr;
	long i;
	for (i = 0;i < GetLength();i++) {
		if (strAll[i] == strSplit[0]) {
			if (IsBegin(strSplit,&strAll[i])) {
				length = i - nNowPos;
				if (length > 0) {
					wchar_t* chr = new wchar_t[length + 1];
					memcpy(chr,&strAll[nNowPos],sizeof(wchar_t) * length);
					chr[length] = L'\0';
					arr->Add(chr);
				}
				nNowPos = i + nShift;
				i = nNowPos;
			}
		}
	}
	length = i - nNowPos;
	if (length > 0) {
		wchar_t* chrr = new wchar_t[length + 1];
		memcpy(chrr,&strAll[nNowPos],sizeof(wchar_t) * length);
		chrr[length] = L'\0';
		arr->Add(chrr);
	}
}

void TString::Trim() {
	wchar_t* str = (wchar_t*)m_lPntStr;
	wchar_t* strNew = new wchar_t[m_lLength + 1];
	register unsigned long i = 0;
	register unsigned long o = 0;

	while (i < m_lLength && (str[i] == L'\r' || str[i] == L'\n' || str[i] == 32 || str[i] == L'\t' || str[i] == 65535)) i++;

	if (str[i] != L'\0') {
		m_lLength = m_lLength - i;
		memcpy(strNew,&str[i],m_lLength * sizeof(wchar_t));
		strNew[m_lLength] = L'\0';
		i = m_lLength - 1;
		while (i >= 0 && (strNew[i] == L'\r' || strNew[i] == L'\n' || strNew[i] == L' ' || strNew[i] == L'\t' || str[i] == 65535)) {
			strNew[i--] = L'\0';
			m_lLength--;
		}
		delete str;
		m_lPntStr = (long)strNew;
	}
	else {
		m_lLength = 0;
		delete str;
		str = new wchar_t;
		str[0] = L'\0';
		m_lPntStr = (long)str;
	}
}

void TString::Replace(const wchar_t* strReplace,const wchar_t* strWith) {
	wchar_t* chr = new wchar_t[m_lLength + 1];
	memcpy(chr,(void*)m_lPntStr,sizeof(wchar_t) * m_lLength);
	chr[m_lLength] = L'\0';
	wchar_t* strBuff = new wchar_t[m_lLength + 1];
	int nEnd = m_lLength;
	Set(L"");
	int i = 0;
	int o = 0;
	int nLength = wcslen(strReplace);
	for (i = 0;i < nEnd;i++) {
		if (IsBegin(strReplace,&chr[i])) {
			strBuff[o] = L'\0';
			o = 0;
			Add(strBuff);
			Add(strWith);
			i += nLength - 1;
			//i++;
		}
		else {
			strBuff[o++] = chr[i];
		}
	}
	strBuff[o] = L'\0';
	Add(strBuff);
	delete strBuff;
	delete chr;
}

void TString::ToUpper() {
	wchar_t* chr = (wchar_t*)m_lPntStr;
	for (register unsigned long i = 0;i < m_lLength;i++) {
		if (chr[i] >= 97 && chr[i] <= 122) chr[i] -= 32;
	}
}

void TString::ToLower() {
	wchar_t* chr = (wchar_t*)m_lPntStr;
	for (register unsigned long i = 0;i < m_lLength;i++) {
		if (chr[i] >= 65 && chr[i] <= 90) chr[i] += 32;
	}
}

const wchar_t* TString::SubStr(unsigned long nStart,unsigned long nLength) {
	if (nStart >= m_lLength) {
		m_lLength = 0;
		delete (wchar_t*)m_lPntStr;
		wchar_t* strNew = new wchar_t[1];
		strNew[0] = L'\0';
		m_lPntStr = (long)strNew;
		return (wchar_t*)m_lPntStr;
	}
	unsigned long nStk = nStart + nLength;
	if (nStk >= m_lLength) nLength = m_lLength - nStart;
	if (nLength == 0) nLength = m_lLength - nStart;
	wchar_t* strNew = new wchar_t[nLength + 1];
	wchar_t* strOld = (wchar_t*)m_lPntStr;
	strOld[nStart + nLength] = L'\0';
	//strcpy(strNew,&strOld[nStart]);
	//strncpy(strNew,&strOld[nStart],nLength + 1);
	memcpy(strNew,&strOld[nStart],sizeof(wchar_t) * nLength);
	strNew[nLength] = L'\0';
	delete (wchar_t*)m_lPntStr;
	m_lPntStr = (long)strNew;
	m_lLength = nLength;
	return (wchar_t*)m_lPntStr;
}

bool TString::IsEQ(const wchar_t* str) {
	wchar_t* strIn = (wchar_t*)m_lPntStr;
	register unsigned long i;
	for (i = 0;str[i] != L'\0';i++) {
		if (strIn[i] != str[i]) return false;
	}
	if (strIn[i] == L'\0' && str[i] == L'\0') return true;
	return false;
}

bool TString::IsEQ(const wchar_t* str1, const wchar_t* str2) {
	register unsigned long i;
	for (i = 0;str1[i] != L'\0';i++) {
		if (str2[i] != str1[i]) return false;
	}
	if (str2[i] == L'\0' && str1[i] == L'\0') return true;
	return false;
}

bool TString::IsEQNC(const wchar_t* str) {
	wchar_t* strIn = (wchar_t*)m_lPntStr;
	register long i;
	int chr1,chr2;
	for (i = 0;str[i] != L'\0';i++) {
		chr1 = (int)strIn[i];
		chr2 = (int)str[i];
		if (chr1 >= 97 && chr1 <= 122) chr1 -= 32;
		if (chr2 >= 97 && chr2 <= 122) chr2 -= 32;
		if (chr1 != chr2) return false;
	}
	if (strIn[i] == L'\0' && str[i] == L'\0') return true;
	return false;
}

int TString::Cmp(const wchar_t* str1, const wchar_t* str2) {
	register long i;
	for (i = 0; str1[i] != L'\0'; i++) {
		if (str1[i] != str2[i]) return str1[i] - str2[i];
	}
	if (str1[i] != str2[i]) return str1[i] - str2[i];
	return 0;
}

bool TString::IsNum() {
	wchar_t* str = (wchar_t*)m_lPntStr;
	long i;
	for (i = 0;i < m_lLength;i++) {
		if (str[i] < 0x30 || str[i] > 0x39) return false;
	}
	return true;
}

bool TString::IsWm(const wchar_t* str) {
	long nMark;
	bool bChk = true;
	TString strMain = (wchar_t*)m_lPntStr;
	wchar_t strStack[5000];
	for (register int i = 0;str[i] != L'\0';i++) {
		if (str[i] == L'*') {
			bChk = false;
		}
		else {
			int nInc = 0;
			while (str[i] != L'*' && str[i] != L'\0') {
				strStack[nInc++] = str[i++];
			}
			strStack[nInc] = L'\0';
			long nChk = strMain.Pos(strStack,1);
			if (nChk < 0) return false;
			if (bChk && nChk > 0) return false;
			bChk = false;
			nMark = nChk + nInc;
			strMain.SubStr(nMark,0);
			if (str[i] == L'\0') {
				int nChk0 = strMain.GetLength();
				if (nChk0 > 0) {
					int nChk = nChk0 - nInc;
					int nChk2 = strMain.Pos(strStack,0);
					int nChk3 = strMain.Pos(strStack,nChk2);
					if (nChk3 == nChk) return true;
					return false;
				}
				else return true;
			}
		}
	}
	return true;
}

wchar_t TString::GetAt(int nIndex) {
	wchar_t* str = (wchar_t*)m_lPntStr;
	if (nIndex >= GetLength()) return str[GetLength() - 1];
	else if (nIndex < 0) return str[0];
	return str[nIndex];
}

wchar_t* TString::Clone() {
	wchar_t* strOut = new wchar_t[m_lLength + 1];
	memcpy(strOut,(void*)m_lPntStr,m_lLength * sizeof(wchar_t));
	strOut[m_lLength] = L'\0';
	return strOut;
}

wchar_t* TString::Pointer() {
	return (wchar_t*)m_lPntStr;
}

void TString::UpdateLength() {
	m_lLength = wcslen((wchar_t*)m_lPntStr);
}
