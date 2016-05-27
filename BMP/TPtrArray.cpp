#include "stdafx.h"
#include "TPtrArray.h"

TPtrArray::TPtrArray()
{
	m_pAll = new long[10];
	m_lLength = 10;
	m_lPtr = 0;
}

TPtrArray::TPtrArray(int n)
{
	m_pAll = new long[n];
	m_lLength = n;
	m_lPtr = 0;
}

TPtrArray::~TPtrArray()
{
	delete m_pAll;
}

long TPtrArray::Add(void *ptr)
{
	if (m_lPtr >= m_lLength) {
		long nNewLength = m_lLength * 2;
		long* ptrNew = new long[nNewLength];
		memcpy(ptrNew,m_pAll,sizeof(long) * m_lLength);
		m_lLength = nNewLength;
		delete m_pAll;
		m_pAll = ptrNew;
	}
	m_pAll[m_lPtr++] = (long)ptr;
	return m_lPtr;
}

long TPtrArray::GetLength()
{
	return m_lPtr;
}

void* TPtrArray::GetAt(long n)
{
	if (n >= m_lPtr) n = m_lPtr - 1;
	return (void*)m_pAll[n];
}

void* TPtrArray::Pop() {
	long n = m_lPtr - 1;
	void* ptr = GetAt(n);
	DelAt(n);
	return ptr;
}

void TPtrArray::SetAt(long n, void *ptr)
{
	if (n < 0) m_pAll[0] = (long)ptr;
	else if (n < m_lPtr) m_pAll[n] = (long)ptr;
	else Add(ptr);
}

void TPtrArray::DestroyString()
{
	for (register long i = 0;i < m_lPtr;i++) delete (wchar_t*)m_pAll[i];
	m_lPtr = 0;
}

long TPtrArray::DelAt(long n)
{
	if (n < m_lPtr) {
		long length = m_lPtr - n - 1;
		if (length > 0) memcpy(&m_pAll[n],&m_pAll[n + 1],sizeof(long) * length);
		m_lPtr--;
	}
	return m_lPtr;
}

long TPtrArray::DelAt(long nBegin, long nEnd)
{
	if (nBegin < m_lPtr) {
		if (nEnd >= m_lPtr) nEnd = m_lPtr - 1;
		long lengthDel = nEnd - nBegin + 1;
		long length = m_lPtr - nEnd - 1;
		if (lengthDel > 0) {
			if (lengthDel == m_lPtr) {
				m_lPtr = 0;
			}
			else {
				memcpy(&m_pAll[nBegin],&m_pAll[nBegin + lengthDel],sizeof(long) * length);
				m_lPtr -= lengthDel;
			}
		}
	}
	return m_lPtr;
}

void TPtrArray::Insert(long nIndex,void* ptr)
{
	if (nIndex >= m_lPtr) {
		Add(ptr);
		return;
	}
	if (m_lPtr >= m_lLength) {
		long nNewLength = m_lLength * 2;
		long* ptrNew = new long[nNewLength];
		memcpy(ptrNew,m_pAll,sizeof(long) * m_lLength);
		m_lLength = nNewLength;
		delete m_pAll;
		m_pAll = ptrNew;
	}
	long* ptrNow = new long[m_lLength];
	memcpy(ptrNow,m_pAll,sizeof(long) * nIndex);
	ptrNow[nIndex] = (long)ptr;
	memcpy(&ptrNow[nIndex + 1],&m_pAll[nIndex],sizeof(long) * (m_lPtr - nIndex));
	m_lPtr++;
	delete m_pAll;
	m_pAll = ptrNow;
}

void TPtrArray::Del(void* p) {
	long i;
	for (i = 0; i < m_lPtr; i++) {
		if ((void*)m_pAll[i] == p) {
			DelAt(i);
			break;
		}
	}
}