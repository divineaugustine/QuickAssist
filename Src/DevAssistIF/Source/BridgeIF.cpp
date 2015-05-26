#include "stdafx.h"
#include <Bridge\BridgeIF.h>

#include <Settings.h>
#include <FileListDlg.h>
#include <DirectoryDlg.h>

#include<Core\Repository.h>
#include<Core\DevAssistFactory.h>
#include <Core\ConfigI.h>


long GetFolder(FileInfo* pFIle, std::wstring& wsFileOut )
{
	ConfigI* pConfig =  DevAssistFactory::Instance().GetConfig();

	std::shared_ptr<DirectoryIterator> iter= pFIle->CreateIterator();
	int nDirCount = pFIle->GetDirectoryCount();
	DirectoryInfo*  pDir  = 0;
	if( 1 == nDirCount )
	{
		pDir = iter->Next();
	}
	else
	{
		CDirectoryDlg obj(iter, pFIle->GetFileName().c_str());
		if( IDOK != obj.DoModal())
		{
			return BRIDGE_ESC;
		}
		pDir = obj.GetDirectory();

		if( !pDir)
			return BRIDGE_UNKNOWN_ERROR;
	}

	BaseDirectory base = pConfig->GetBaseDirectory();
	TCHAR tcszBuff[2048];
	base.ResolveVirtualPath( pDir, tcszBuff );
	std::wstring wsName(tcszBuff ) ;
	wsFileOut = wsName+ pFIle->GetFileName();
	return BRIDGE_SUCCESS;
}

long BRIDGE_API ToggleSoruce( LPCWSTR fileName, int nOutBufferSize_i, LPWSTR lpFile_o )
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DevAssistFactory& obj = DevAssistFactory::Instance();
	RepositoryBase* pRep = obj.GetRepository();
	//static bool bFirst = true;
	//if(bFirst)
	//{
	//	SerialStrategyBase* pStrat = obj.GetSerializationStrategy();
	//	RepositoryBase* pRep = obj.GetRepository();
	//	pRep->Deserialize( pStrat );
	//	bFirst = false;
	//}

	std::wstring wsCurrentFile(fileName);
	std::wstring hpp(L".hpp");
	std::wstring cpp(L".cpp");
	std::wstring h(L".h");

	FileInfo* pFIle = 0;
	PTSTR strExt = PathFindExtension( fileName );
	std::wstring wsExt = strExt;
	if(wsExt == hpp )
	{
		std::wstring temp(wsCurrentFile);
		temp.replace(temp.find(hpp),hpp.length(),cpp );
		pFIle = pRep->GetFile(temp.c_str());
	}
	else if(wsExt == h )
	{
		std::wstring temp(wsCurrentFile);
		temp.replace(temp.find(h),h.length(),cpp );
		pFIle = pRep->GetFile(temp.c_str());
	}
	else if(wsExt == cpp )
	{
		std::wstring temp(wsCurrentFile);
		temp.replace(temp.find(cpp),cpp.length(),hpp );
		pFIle = pRep->GetFile(temp.c_str());
		if( !pFIle )
		{
			temp.replace(temp.find(hpp),hpp.length(),h );
			pFIle = pRep->GetFile(temp.c_str());
		}
	}

	if( pFIle )
	{
		std::wstring wsFolder;
		int nRet = GetFolder( pFIle, wsFolder );
		lstrcpy( lpFile_o, wsFolder.c_str());
		return nRet;
	}
	return BRIDGE_NO_TOGGLE;
}

long BRIDGE_API OpenSettings()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CSettings obj;
	obj.DoModal();
	return BRIDGE_SUCCESS;
}



long BRIDGE_API SearchFile( LPCWSTR fileName, int nOutBufferSize_i, LPWSTR lpFile_o )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//static bool bFirst = true;
	//if(bFirst)
	//{
	//	DevAssistFactory& obj = DevAssistFactory::Instance();
	//	SerialStrategyBase* pStrat = obj.GetSerializationStrategy();
	//	RepositoryBase* pRep = obj.GetRepository();
	//	pRep->Deserialize( pStrat );
	//	bFirst = false;
	//}

	DevAssistFactory& obj = DevAssistFactory::Instance();
	RepositoryBase* pRep = obj.GetRepository();
	ConfigI* pConfig = obj.GetConfig();

	FileInfo* pFIle = 0;
	
	if(fileName)
	{
		pFIle = pRep->GetFile(fileName);
	}

	if( !pFIle )
	{
		FileListDlg obj;
		obj.setSearch(fileName);
		if( IDOK != obj.DoModal())
		{
			return BRIDGE_ESC;
		}
		pFIle = obj.GetSelectedFile();
	}
	if( !pFIle )
	{
		return BRIDGE_UNKNOWN_ERROR;
	}

	std::wstring wsFileOut;
	int nRet = GetFolder( pFIle, wsFileOut );
	lstrcpy( lpFile_o, wsFileOut.c_str());
	return nRet;
}