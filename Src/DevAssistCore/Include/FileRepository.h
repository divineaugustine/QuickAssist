#pragma once
#include <Core\Repository.h>
#include <map>
#include <vector>
class FileInfoImpl;
class DirectoryInfoImpl;

// Folder information will be stored after removing the configurable path
class FileRepository : public RepositoryBase
{
public:

	DirectoryInfo* AddFolder( const std::wstring& strFolder_i );
	void AddFile( const std::wstring& strFile_i, DirectoryInfo* pDirInfo );

	bool Serialize( SerialStrategyBase*  pSerialStrategy );
	bool Deserialize( SerialStrategyBase*  pSerialStrategy );

	bool WriteToFile( const std::wstring& strDestFile_i );
	bool ReadFromFile( const std::wstring& strDestFile_i );

	virtual std::shared_ptr<FileIterator> CreateFileIterator();
	FileInfo* GetFile(  const std::wstring& FileName_i );

	void ResetContents();

	FileRepository();
	~FileRepository();
private:
	FileRepository(const FileRepository& src);
	FileRepository& operator=(const FileRepository& src);

	void ClearAll();

private:

	// std::map<std::wstring,std::vector<unsigned int>*> mFileRep;
	std::map<std::wstring,FileInfoImpl*> mFileRep;
	std::vector<DirectoryInfoImpl*> mFolderRep;
	unsigned int mnFolderCounter;
};