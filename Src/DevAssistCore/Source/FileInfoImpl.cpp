#include "stdafx.h"
#include "FileInfoImpl.h"
#include <DirectoryIteratorImpl.h>

FileInfoImpl::FileInfoImpl( const std::wstring csFileName_i )
	:mcsFileName(csFileName_i)

{
}
FileInfoImpl::~FileInfoImpl(){}
const std::wstring& FileInfoImpl::GetFileName() const
{
	return mcsFileName;
}

int FileInfoImpl::GetDirectoryCount() 
{
	return mpnDirArray.size();
}

std::shared_ptr<DirectoryIterator> FileInfoImpl::CreateIterator()
{
	std::shared_ptr<DirectoryIterator> itr =  std::make_shared<DirectoryIteratorImpl>(&mpnDirArray[0], 
												mpnDirArray.size());
	// new DirectoryIteratorImpl( &mpnDirArray[0], mpnDirArray.size());
	return itr;
}

void FileInfoImpl::AddDirectory(DirectoryInfo* info)
{
	mpnDirArray.push_back(info);
}