#pragma once 
#include <string>
#include <vector>
#include "Defs.h"
class DirectoryInfo;


class CORE_API RepositoryName
{
public:

	const std::wstring& GetName() const;
	void SetName(const std::wstring& csName);
	RepositoryName(  );
	~RepositoryName();
	bool IsValid();

	bool operator == ( const RepositoryName& src);
	bool operator != ( const RepositoryName& src);
	bool operator<(const RepositoryName& name) const;

private:

	std::wstring mcsName;
	
};

class CORE_API BaseDirectory
{
public:

	const std::wstring& GetPath() const ; 
	void SetPath(const std::wstring& cspath);

	int ResolveVirtualPath( DirectoryInfo* pDirInfo_i, TCHAR* pBuff_o );
	bool IsValid() ;
	BaseDirectory( );
	~BaseDirectory();
	bool operator == ( const BaseDirectory& src);
	bool operator != ( const BaseDirectory& src);
private:

	std::wstring mcsPath;
};


class CORE_API RepositorySettings
{
public:

	const std::vector<std::wstring>& GetConfiguredFolders() const;
	void SetConfiguredFolders( std::vector<std::wstring>& vecConfiguredFolderList );

	const std::vector<std::wstring>& GetSupportedExtensions()  const;
	void SetSupportedExtensions(std::vector<std::wstring>& vecSupportedExtns );

	const std::vector<std::wstring>& GetIgnoreDirectories()  const;
	void SetIgnoreDirectories(std::vector<std::wstring>& vecIgnoreDirs );

	// bool GetNextExtensionChain( std::wstring& csCurrentExt_i, WCHAR* NextExtBuff );
private:

	std::vector<std::wstring> mvecConfigFOlders;
	std::vector<std::wstring> mvecIgnoreDirs;
	std::vector<std::wstring> mvecExtensions;
};


class CORE_API ConfigI
{
public:

	virtual std::wstring& GetInstalledPath() = 0;
	virtual BaseDirectory& GetBaseDirectory() = 0;
	virtual RepositoryName& GetCurrentRepository() = 0;

	virtual const std::vector<RepositoryName>& GetAllRespositories() = 0;
	virtual const std::vector<BaseDirectory>& GetPrevBaseDirectories() = 0;

	virtual bool SetBaseDirectory( const BaseDirectory& csFixedPath_i ) = 0;
	virtual bool SetRepositoryFilename( const RepositoryName& csConfFileName_i ) = 0;

	virtual bool GetRepositorySettings( const RepositoryName& RepName_i, RepositorySettings& out ) = 0;
	virtual void SetRepositorySettings( const RepositoryName& RepName_i, RepositorySettings* pRepoSettings ) = 0;

	virtual bool CreateRepository( const RepositoryName& RepName_i,RepositorySettings& rep ) = 0;
	virtual bool DeleteRepository( const RepositoryName& RepName_i ) = 0;

protected:

	ConfigI(){}
	virtual ~ConfigI(){};

private:

	ConfigI(const ConfigI& src);
	ConfigI& operator=(const ConfigI& src);
};