#include "stdafx.h"
#include "DirectoryInfoImpl.h"



	DirectoryInfoImpl::DirectoryInfoImpl( const std::wstring& csFolderPath, int nDirIndex_i ) : mcsFolderPath(csFolderPath), mnDirIndex(nDirIndex_i)
	{
	}

	DirectoryInfoImpl::~DirectoryInfoImpl(){}
	const std::wstring& DirectoryInfoImpl::GetPath()
	{
		return mcsFolderPath;
	}
	int DirectoryInfoImpl::GetIndex()
	{
		return mnDirIndex;
	}