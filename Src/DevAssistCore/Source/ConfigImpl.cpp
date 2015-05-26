#include <stdafx.h>
#include <ConfigImpl.h>
#include <tchar.h>

	LPCTSTR DEVASSIST_REG_ROOT = _T( "SOFTWARE\\DevAssist" );
	// under user
	LPCTSTR DEVASSIST_BASE_FOLDER =_T( "BaseDirectory" );
	LPCTSTR DEVASSIST_REPO_FILE  = _T( "CurrentRepository" );

	// LPCTSTR DEVASSIST_REPO_HISTORY  = _T( "RepositoryHistory" );
	LPCTSTR DEVASSIST_BASE_HISTORY  = _T( "BaseFolderHistory" );

	LPCTSTR DEVASSIST_REPOSITORY  = _T( "\\REPOSITORY\\" );
	LPCTSTR DEVASSIST_PROPERTYFILENAME  = _T( "\\Settings.prop" );
	LPCTSTR DEVASSIST_MASTER_SETTINGS  = _T( "\\master.prop" );


	bool ConfigImpl::UpdateRegistry( HKEY BASE_KEY, LPCTSTR csKey, std::wstring csVal )
	{
		const int ZERO_CVAL = 0;
		DWORD dValLength = 0;
		DWORD dType	     = 0;
		DWORD dwDisp = 0;
		LPDWORD lpdwDisp = &dwDisp;

		HKEY hKey;

		long lResult = RegCreateKeyEx( BASE_KEY, DEVASSIST_REG_ROOT, 
			0L,0,REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hKey,lpdwDisp );
		if( ERROR_SUCCESS != lResult )
		{
			return false;
		}

		if( ERROR_SUCCESS != RegSetValueEx(hKey, csKey,0,REG_SZ, (LPBYTE) csVal.c_str(), csVal.size()*sizeof

(TCHAR)))
		{
			RegCloseKey( hKey);
			return false;
		}
		RegCloseKey( hKey);
		return true;
	}

	bool ConfigImpl::GetRegistry( HKEY BASE_KEY, const TCHAR* csKey, std::wstring& csVal )
	{

		const int ZERO_CVAL = 0;
		DWORD dValLength = MAX_PATH-1;
		DWORD dType	     = 0;
		DWORD dwDisp = 0;
		LPDWORD lpdwDisp = &dwDisp;
		TCHAR szDriveName[MAX_PATH]={'\0'};
		HKEY hKey;
		long lResult = RegOpenKeyEx(BASE_KEY, \
						DEVASSIST_REG_ROOT, \
						ZERO_CVAL, \
						KEY_QUERY_VALUE, \
						&hKey);                
		if(ERROR_SUCCESS != lResult)
		{
			return false;
		}

		lResult = RegQueryValueEx(\
			hKey, \
			csKey, \
			NULL, \
			&dType, \
			(BYTE *)szDriveName, \
			&dValLength);

		if((ERROR_SUCCESS != lResult) && (ERROR_FILE_NOT_FOUND != lResult))
		{
			return false;
		}
		else
		{
			csVal = szDriveName;
			return true;
		}
	}

	void FillHistoryVector( std::wstring historyString_i, std::vector<BaseDirectory>& lstReporitories_o )
	{
		wchar_t* token = std::wcstok(const_cast<wchar_t*>(historyString_i.c_str()), L";");
		while (token) 
		{
			BaseDirectory stBase;
			stBase.SetPath( token );
			lstReporitories_o.push_back( stBase );
			token = std::wcstok(nullptr, L";");			
		}
	}
	//void FillHistoryVector( std::wstring historyString_i, std::vector<RepositoryName>& lstReporitories_o )
	//{
	//	wchar_t* token = std::wcstok(const_cast<wchar_t*>(historyString_i.c_str()), L";");
	//	while (token) 
	//	{
	//		RepositoryName stBase;
	//		stBase.SetName( token );
	//		lstReporitories_o.push_back( stBase );
	//		token = std::wcstok(nullptr, L";");			
	//	}
	//}

	EXTERN_C IMAGE_DOS_HEADER __ImageBase;
	ConfigImpl::ConfigImpl()
	{
		int n = 0;
		std::wstring wsVal;
		GetRegistry( HKEY_CURRENT_USER, DEVASSIST_BASE_FOLDER, wsVal );
		mcsBaseDirectory.SetPath( wsVal );
		GetRegistry( HKEY_CURRENT_USER, DEVASSIST_REPO_FILE, wsVal );
		mcsRespositoryFileName.SetName( wsVal );

		WCHAR   DllPath[MAX_PATH] = {0};
		GetModuleFileNameW((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
		PathRemoveFileSpec(DllPath);

		mcsInstallPath = DllPath;

		// GetRegistry( HKEY_LOCAL_MACHINE, DEVASSIST_INSTALL_DIR, mcsInstallPath );

		 std::wstring wsHistory;
		// GetRegistry( HKEY_CURRENT_USER, DEVASSIST_REPO_HISTORY, wsHistory );
		// FillHistoryVector(wsHistory, mcsRepoHistory);
		
		GetRegistry( HKEY_CURRENT_USER, DEVASSIST_BASE_HISTORY, wsHistory );
		FillHistoryVector(wsHistory, mcsBaseFolderHistory);
		InitRepositoryNames();

		// Init the repository settings
		InitRepoSettings();
	}

	std::wstring& ConfigImpl::GetInstalledPath()
	{
		return mcsInstallPath;
	}

	BaseDirectory& ConfigImpl::GetBaseDirectory()
	{
		return mcsBaseDirectory;
	}

	RepositoryName& ConfigImpl::GetCurrentRepository()
	{
		return mcsRespositoryFileName;
	}
	const std::vector<RepositoryName>& ConfigImpl::GetAllRespositories() 
	{
		return mcsRepoList;
	}
	const std::vector<BaseDirectory>& ConfigImpl::GetPrevBaseDirectories()
	{
		return mcsBaseFolderHistory;
	}

	#define max_history 10

	//void UpdateHistory( RepositoryName& wsOld, RepositoryName& wsNew, std::vector<RepositoryName>& history_io )
	//{
	//	if( wsOld.IsValid() != 0 )
	//	{
	//		if( max_history == history_io.size())
	//		{
	//			history_io.erase( history_io.begin());
	//		}
	//		history_io.push_back(wsOld);
	//	}
	//	if( wsNew.IsValid() != 0 )
	//	{
	//		for (std::vector<RepositoryName>::iterator it = history_io.begin() ; it != history_io.end(); ++it)
	//		{
	//			if( wsNew == (*it))
	//			{
	//				history_io.erase(  it );
	//				break;
	//			}
	//		}
	//	}
	//}
	void UpdateHistory(  BaseDirectory& wsOld,  BaseDirectory& wsNew, std::vector<BaseDirectory>& history_io )
	{
		if( wsOld.IsValid() != 0 )
		{
			if( max_history == history_io.size())
			{
				history_io.erase( history_io.begin());
			}
			history_io.push_back(wsOld);
		}
		if( wsNew.IsValid() != 0 )
		{
			for (std::vector<BaseDirectory>::iterator it = history_io.begin() ; it != history_io.end(); ++it)
			{
				if( wsNew == (*it))
				{
					history_io.erase(  it );
					break;
				}
			}
		}
	}

	void GetHistoryString(  std::vector<BaseDirectory>& history_i, std::wstring& history_o )
	{
		for ( std::vector<BaseDirectory>::iterator it = history_i.begin() ; it != history_i.end(); ++it)
		{
			history_o += (*it).GetPath();
			history_o += L";";				
		}
	}
	//void GetHistoryString(  std::vector<RepositoryName>& history_i, std::wstring& history_o )
	//{
	//	for ( std::vector<RepositoryName>::iterator it = history_i.begin() ; it != history_i.end(); ++it)
	//	{
	//		history_o += (*it).GetName();
	//		history_o += L";";				
	//	}
	//}


	bool ConfigImpl::SetBaseDirectory( const BaseDirectory& csBase )
	{
		BaseDirectory csBase_i(csBase);
		if( csBase_i.IsValid() == 0 )
		{
			return false;
		}

		if( mcsBaseDirectory != csBase_i )
		{
			UpdateHistory( mcsBaseDirectory, csBase_i, mcsBaseFolderHistory );
			mcsBaseDirectory = csBase_i;
			UpdateRegistry( HKEY_CURRENT_USER, DEVASSIST_BASE_FOLDER, mcsBaseDirectory.GetPath());
			std::wstring wsHistory;
			GetHistoryString( mcsBaseFolderHistory, wsHistory );
			return UpdateRegistry( HKEY_CURRENT_USER, DEVASSIST_BASE_HISTORY, wsHistory );
		}
		return true;
	}

	bool ConfigImpl::SetRepositoryFilename( const RepositoryName& csConfFileName )
	{
		RepositoryName csConfFileName_i(csConfFileName);
		if( csConfFileName_i.GetName().size() == 0 )
		{
			return false;
		}

		if( mcsRespositoryFileName != csConfFileName_i)
		{
			// UpdateHistory( mcsRespositoryFileName, csConfFileName_i, mcsRepoHistory );
			mcsRespositoryFileName = csConfFileName_i;
			UpdateRegistry( HKEY_CURRENT_USER, DEVASSIST_REPO_FILE, mcsRespositoryFileName.GetName() );
			return true;
			// std::wstring wsHistory;
			// GetHistoryString( mcsRepoHistory, wsHistory );
			// return UpdateRegistry( HKEY_CURRENT_USER, DEVASSIST_REPO_HISTORY, wsHistory );
		}
		return true;
	}


#include <settingsReader.h>
	// RepositorySettings ConfigImpl::GetRepositorySettings( const RepositoryName& RepName_i )
	bool ConfigImpl::GetRepositorySettings( const RepositoryName& RepName_i, RepositorySettings& out )
	{
		auto search = mSettings.find(RepName_i);
		if(search != mSettings.end()) 
		{
			out = search->second;
			return true;
		}
		else
		{
			return false;
		}
	}

	std::wstring GetRepoPath( std::wstring wsInstalledPath_i, const RepositoryName* pName_i )
	{
		std::wstring wsFolder(wsInstalledPath_i);
		wsFolder += DEVASSIST_REPOSITORY;
		wsFolder += pName_i->GetName();
		// wsFolder += L"\\Settings.prop";
		return wsFolder;
	}

	void ConfigImpl::SetRepositorySettings( const RepositoryName& RepName_i, RepositorySettings* pRepoSettings )
	{
		/*auto search = mSettings.find(RepName_i);
		if(search != mSettings.end()) 
		{
			RepositorySettings* pRp = search->second;
			delete pRp;
		}*/

		RepositorySettings pRp(*pRepoSettings);
		mSettings[RepName_i] = pRp;

		std::wstring wsFolder = GetRepoPath( mcsInstallPath, &RepName_i );
		wsFolder += DEVASSIST_PROPERTYFILENAME;

		CSettingsReader::Instance().WriteSettings( pRp, wsFolder );
		return;
	}



	bool ConfigImpl::InitRepoSettings()
	{
		mSettings.clear();

		for ( std::vector<RepositoryName>::iterator it = mcsRepoList.begin() ; it != mcsRepoList.end(); ++it)
		{
			std::wstring wsFolder = GetRepoPath( mcsInstallPath, (RepositoryName*)&(*it) );
			wsFolder += DEVASSIST_PROPERTYFILENAME;
			RepositorySettings pRp;

			CSettingsReader::Instance().ReadSettings( wsFolder, pRp );
			mSettings[(*it)] = pRp;
		}
		return true;
	}


	bool ConfigImpl::CreateRepository( const RepositoryName& RepName_i,RepositorySettings& rep )
	{
		// RepositorySettings pRp;

		// Check if its already available
		auto search = mSettings.find(RepName_i);
		if(search != mSettings.end()) 
		{
			return 0;
		}

		std::wstring wsFolder = GetRepoPath( mcsInstallPath, &RepName_i );
		// Create a folder with given name under the repository folder
		if( FALSE == CreateDirectory( wsFolder.c_str(), 0 ))
		{
			return 0;
		}

		std::wstring wsMasterFile = mcsInstallPath + DEVASSIST_MASTER_SETTINGS;
		wsFolder += DEVASSIST_PROPERTYFILENAME;		
		if( FALSE == CopyFile(  wsMasterFile.c_str(), wsFolder.c_str(), FALSE  ))
		{
			return 0;
		}

		
		CSettingsReader::Instance().ReadSettings( wsFolder, rep );
		mSettings[RepName_i] = rep;
		mcsRepoList.push_back( RepName_i );
		return true;
	}

	bool ConfigImpl::InitRepositoryNames()
	{
		mcsRepoList.clear();

		CFileFind finder;
		// build a string with wildcards
		// CString strWildcard(pstr);
		CString csPath(mcsInstallPath.c_str()+CString(DEVASSIST_REPOSITORY) );
		csPath.TrimRight( _T( "\\" ));
		csPath += _T("\\*.*");

		// start working for files
		BOOL bWorking = finder.FindFile(csPath);
		if( !bWorking )
		{
			return false;
		}

		bool bFirst = true;
		UINT uPath = 0;
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files; 

			if (finder.IsDots() || finder.IsSystem() || finder.IsHidden() || finder.IsTemporary() )
				continue;

			// if it's a directory, recursively search it 			
			if (finder.IsDirectory())
			{
				std::wstring wsName(finder.GetFileName());
				RepositoryName stName;
				stName.SetName(wsName );
				mcsRepoList.push_back(stName);
			}
		}
		finder.Close();
	}


	bool ConfigImpl::DeleteRepository( const RepositoryName& RepName_i )
	{
		// Check if its already available
		auto search = mSettings.find(RepName_i);
		if(search == mSettings.end()) 
		{
			return 0;
		}

		std::wstring wsFolder = GetRepoPath( mcsInstallPath, &RepName_i );
		std::wstring wsProp = wsFolder + DEVASSIST_PROPERTYFILENAME;

		std::wstring wsRep  = wsFolder + L"\\FileList.rpo";
		DeleteFile( wsRep.c_str());
		DeleteFile( wsProp.c_str());
		RemoveDirectory( wsFolder.c_str());

		InitRepositoryNames();
		InitRepoSettings();
		return true;
	}