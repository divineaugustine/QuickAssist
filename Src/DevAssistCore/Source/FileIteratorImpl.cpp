#include "stdafx.h"
#include "FileIteratorImpl.h"
#include "FileInfoImpl.h"

	FileIteratorImpl::FileIteratorImpl( std::map<std::wstring,FileInfoImpl*>* pFileRep)
					  :mpFileRep(pFileRep)
	{
		mBegin = pFileRep->begin();
	};

	FileInfo* FileIteratorImpl::Next( )
	{
		FileInfo* pInfo = mBegin->second;
		++mBegin;
		return pInfo;
	}

	bool FileIteratorImpl::HasNext()
	{
		return mBegin == mpFileRep->end() ? false: true;
	
	}

	FileIteratorImpl::~FileIteratorImpl(){}