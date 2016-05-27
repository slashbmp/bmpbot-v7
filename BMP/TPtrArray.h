class TPtrArray {
public:
	void Insert(long,void*);
	long DelAt(long,long);
	long DelAt(long);
	void Del(void*);
	void DestroyString();
	void SetAt(long,void*);
	void* GetAt(long);
	long GetLength();
	long Add(void*);
	void* Pop();
	TPtrArray();
	TPtrArray(int);
	virtual ~TPtrArray();

protected:
	long m_lPtr;
	long m_lLength;
	long* m_pAll;
};
