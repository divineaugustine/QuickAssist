#include "stdafx.h"
#include "DirectoryIteratorImpl.h"


	DirectoryIteratorImpl::DirectoryIteratorImpl( DirectoryInfo** pArray, int nMaxLength ) 
		:mnCurrentIndx(0)
		,mpArray(pArray)
		, mnMax(nMaxLength)
	{
	};

DirectoryIteratorImpl::DirectoryIteratorImpl(void)
{
}


DirectoryIteratorImpl::~DirectoryIteratorImpl(void)
{
}

	DirectoryInfo* DirectoryIteratorImpl::Next( ) 
	{
		return mpArray[mnCurrentIndx++];
	}

	bool DirectoryIteratorImpl::HasNext() 
	{
		bool ret = mnCurrentIndx <mnMax ? true:false;
		return ret;
	}