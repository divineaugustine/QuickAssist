#include "stdafx.h"
#include <DirectoryParserMgr.h>
// #include "ConfigReader.h"


#include <Core\DevAssistFactory.h>
#include <Core\ConfigI.h>
#include <Core\Repository.h>

CDirectoryParserMgr& CDirectoryParserMgr::Instance()
{
	static CDirectoryParserMgr obj;
	return obj;
}

bool CDirectoryParserMgr::IsIgnore( LPCTSTR lpFoledr  )
{
	CString csInput(lpFoledr);
	csInput.MakeUpper();
	int nVal = 0;
	mIgnoreFOlders.Lookup( csInput, nVal );
	return nVal == 1 ? true: false;
}

bool CDirectoryParserMgr::ParseFolder( LPCTSTR lpFolder_i, 
									   LPCTSTR lpVirtualFolder_i  )
{
	USES_CONVERSION;
	CStringArray objFiles;
	CStringArray objZFolders;

	if( !mobjParse.ParseDirectory( lpFolder_i , objZFolders, objFiles ))
	{
		if( mfnCallBack )
		{
			CString csError( _T( "Error :- " ));
			csError += lpFolder_i;
			mfnCallBack( csError );
			return false;
		}
	}


	DevAssistFactory& obj = DevAssistFactory::Instance();
	RepositoryBase* pRepo = obj.GetRepository();

	DirectoryInfo* nFolderID = 0;
	if( objFiles.GetSize())
	{
		nFolderID = pRepo->AddFolder( lpVirtualFolder_i );
	}
	

	for( int nIndx = 0; nIndx < objFiles.GetSize();++nIndx)
	{
		LPCTSTR lpFile = objFiles.GetAt(nIndx);	
		pRepo->AddFile(lpFile, nFolderID);
	}


	for( int nIndx = 0; nIndx < objZFolders.GetSize();++nIndx)
	{
		LPCTSTR lpFolder = objZFolders.GetAt(nIndx);	
		if( IsIgnore( lpFolder ))
		{
			continue;
		}

		CString csVirtual( lpVirtualFolder_i );
		csVirtual += lpFolder ;
		csVirtual += _T( "\\" );

		CString csActual( lpFolder_i );
		csActual += lpFolder ;
		csActual += _T( "\\" );

		ParseFolder(csActual, csVirtual );
		if( mfnCallBack )
		{
			CString csError( _T( "Done " ));
			csError += csActual;
			mfnCallBack( csError );
		}
	}
	return true;
}


bool CDirectoryParserMgr::StartParsing( )
{
	DevAssistFactory& obj = DevAssistFactory::Instance();
	ConfigI* pConfig = obj.GetConfig();
	RepositorySettings mpSettings;
	RepositoryBase* pRepo = obj.GetRepository();
	pRepo->ResetContents();

	if( !pConfig->GetRepositorySettings( pConfig->GetCurrentRepository(), mpSettings ))
	{
		if( mfnCallBack )
		{
			CString csError( _T( "Current repository not defined \n" ));
			mfnCallBack( csError );
		}
		return 0;
	}

	const std::vector<std::wstring>& Ignore = mpSettings.GetIgnoreDirectories();
	for ( std::vector<std::wstring>::const_iterator  it = Ignore.begin() ; it != Ignore.end(); ++it )
	{
		mIgnoreFOlders.SetAt( (*it).c_str(), 1 );
	}

	const std::vector<std::wstring>& Extensions = mpSettings.GetSupportedExtensions();

	for ( std::vector<std::wstring>::const_iterator  it = Extensions.begin() ; it != Extensions.end(); ++it )
	{
		mobjParse.AddInterestedExtensions((*it).c_str());
	}
	const std::vector<std::wstring>& Folders = mpSettings.GetConfiguredFolders();
	if( 0<Folders.size())
	{
		for ( std::vector<std::wstring>::const_iterator  it = Folders.begin() ; it != Folders.end(); ++it )
		{
			CString csSub((*it).c_str());
			csSub.TrimRight(_T( "\\" ));
			csSub.TrimLeft(_T( "\\" ));

			// csPath += _T( "\\" );

			CString csActual;
			csActual.Format(L"%s\\%s\\",mcsReplaceStatic, csSub );
			
			CString csVirtual;
			csVirtual.Format(L"?\\%s\\", csSub );
			// = csPath;
			// csActual.Replace( _T( "?" ), mcsReplaceStatic );
			ParseFolder(csActual, csVirtual );
		}
	}
	// Include all folders
	else
	{
		CString csVirtual(L"?\\");
		// csPath.TrimRight(_T( "\\" ));
		// csPath += _T( "\\" );

		CString csActual = csVirtual;
		csActual.Replace( _T( "?" ), mcsReplaceStatic );
		ParseFolder(csActual, csVirtual );
	}


	
	SerialStrategyBase* pStrat = obj.GetSerializationStrategy();
	if( pRepo->Serialize( pStrat ))
	{
		mfnCallBack( L"Indexing done !" );
	}
	else
	{
		mfnCallBack( L"Indexing failed !" );
	}
	return false;
}
