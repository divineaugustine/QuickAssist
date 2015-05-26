#pragma once
#include <string>
#include <memory>
#include <Core\DataStructs.h>
#include "Defs.h"

class FileRepository;
class DirectoryInfo;
class FileInfo;

class CORE_API SerialStrategyBase
{
public:
	virtual bool Serialize(  FileRepository* pFile ) = 0;
	virtual bool Deserialize( FileRepository* pFile ) = 0;
protected:
	SerialStrategyBase(){}
	virtual ~SerialStrategyBase(){};
private:
	SerialStrategyBase(const SerialStrategyBase& src);
	SerialStrategyBase& operator=(const SerialStrategyBase& src);
};

class CORE_API RepositoryBase
{
public:

	virtual DirectoryInfo* AddFolder( const std::wstring& strFolder_i ) = 0;
	virtual void AddFile( const std::wstring& strFile_i, DirectoryInfo* pDirInfo ) = 0;

	virtual bool Serialize( SerialStrategyBase*  pSerialStrategy ) = 0;
	virtual bool Deserialize( SerialStrategyBase*  pSerialStrategy ) = 0;

	virtual std::shared_ptr<FileIterator> CreateFileIterator() = 0;
	virtual FileInfo* GetFile( const std::wstring& FileName_i ) = 0;

	virtual void ResetContents() = 0;

protected:

	RepositoryBase(){}
	virtual ~RepositoryBase(){};

private:

	RepositoryBase(const RepositoryBase& src);
	RepositoryBase& operator=(const RepositoryBase& src);
};