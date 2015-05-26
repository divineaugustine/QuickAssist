#pragma once
#include <Core\DataStructs.h>
class DirectoryIteratorImpl : public DirectoryIterator
{
public:
	
	DirectoryIteratorImpl( DirectoryInfo** pArray, int nMaxLength );
	~DirectoryIteratorImpl();
	DirectoryInfo* Next( ) ;
	bool HasNext() ;

private:
	DirectoryIteratorImpl();
	DirectoryIteratorImpl(const DirectoryIteratorImpl& src);
	DirectoryIteratorImpl& operator=(const DirectoryIteratorImpl& src);

private:

	DirectoryInfo** mpArray;
	int mnCurrentIndx;
	int mnMax;
};
