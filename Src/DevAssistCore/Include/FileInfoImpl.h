#pragma once
#include <Core\Datastructs.h>
#include <vector>
#include <memory>

class FileInfoImpl : public FileInfo
{
public:

	FileInfoImpl( const std::wstring csFileName_i );
	~FileInfoImpl();
	const std::wstring& GetFileName() const;
	int GetDirectoryCount() ;
	std::shared_ptr<DirectoryIterator> CreateIterator();
	void AddDirectory(DirectoryInfo* info);

private:

	FileInfoImpl();
	FileInfoImpl(const FileInfoImpl& src);
	FileInfoImpl& operator=(const FileInfoImpl& src);

private:

	const std::wstring mcsFileName;
	// DirectoryInfo* mpnDirArray;
	std::vector<DirectoryInfo*> mpnDirArray;
	// unsigned int mnDirCount;
};