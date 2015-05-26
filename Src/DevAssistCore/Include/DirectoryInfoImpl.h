#pragma once
#include <Core\Datastructs.h>
class DirectoryInfoImpl : public DirectoryInfo
{
public:

	DirectoryInfoImpl( const std::wstring& csFolderPath, int nDirIndex_i );

	~DirectoryInfoImpl();
	const std::wstring& GetPath();
	int GetIndex();

private:
	DirectoryInfoImpl();
	DirectoryInfoImpl(const DirectoryInfoImpl& src);
	DirectoryInfoImpl& operator=(const DirectoryInfoImpl& src);

private:

	int mnDirIndex;
	const std::wstring mcsFolderPath;
};