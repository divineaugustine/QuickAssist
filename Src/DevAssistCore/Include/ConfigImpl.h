#pragma once
#include <Core\ConfigI.h>

#include <vector>
#include <map>
class ConfigImpl : public ConfigI
{
public:

	std::wstring& GetInstalledPath();

	BaseDirectory& GetBaseDirectory();
	RepositoryName& GetCurrentRepository() ;

	const std::vector<RepositoryName>& GetAllRespositories() ;
	const std::vector<BaseDirectory>& GetPrevBaseDirectories();

	bool SetBaseDirectory( const BaseDirectory& csBase_i );
	bool SetRepositoryFilename( const RepositoryName& csConfFileName_i );

	 bool GetRepositorySettings( const RepositoryName& RepName_i, RepositorySettings& out );
	// RepositorySettings GetRepositorySettings( const RepositoryName& RepName_i );
	void SetRepositorySettings( const RepositoryName& RepName_i, RepositorySettings* pRepoSettings );

	bool CreateRepository( const RepositoryName& RepName_i,RepositorySettings& rep );
	bool DeleteRepository( const RepositoryName& RepName_i );

	ConfigImpl();
	virtual ~ConfigImpl(){};

private:

	ConfigImpl(const ConfigImpl& src);
	ConfigImpl& operator=(const ConfigImpl& src);

	bool UpdateRegistry( HKEY BASE_KEY, LPCTSTR csKey, std::wstring csVal );
	bool GetRegistry( HKEY BASE_KEY, const TCHAR* , std::wstring& csVal );

	bool InitRepositoryNames();
	bool InitRepoSettings();
	
private:

	std::wstring mcsInstallPath;
	BaseDirectory mcsBaseDirectory;
	RepositoryName mcsRespositoryFileName;

	std::vector<RepositoryName> mcsRepoList;
	std::vector<BaseDirectory> mcsBaseFolderHistory;

	std::map<RepositoryName, RepositorySettings> mSettings;
};