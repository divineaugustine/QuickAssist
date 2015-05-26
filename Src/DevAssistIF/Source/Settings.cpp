// Settings.cpp : implementation file
//

#include "stdafx.h"
#include "DevAssistIF.h"
#include "Settings.h"
#include "afxdialogex.h"

#include "..\Resource.h"
#include <Core\DevAssistFactory.h>
#include <Core\ConfigI.h>
#include <Core\Repository.h>

// CSettings dialog

IMPLEMENT_DYNAMIC(CSettings, CDialogEx)

CSettings::CSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettings::IDD, pParent)
{

}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, mCombBaseDir);
	DDX_Control(pDX, IDC_COMBO2, mCombRespository);
}


BEGIN_MESSAGE_MAP(CSettings, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &CSettings::OnReloadRepository)
	ON_BN_CLICKED(IDC_BUTTON5, &CSettings::OnBnClickedOpnSettings)
	ON_BN_CLICKED(IDC_BUTTON4, &CSettings::OnBnClickedUpdateRepository )
	ON_BN_CLICKED(IDC_BUTTON1, &CSettings::OnNewBaseDirectory)
	ON_BN_CLICKED(IDC_BUTTON2, &CSettings::OnCreateNewRepository)
	ON_BN_CLICKED(IDC_BUTTON6, &CSettings::OnBnClickedButton6)
END_MESSAGE_MAP()


// CSettings message handlers

LPITEMIDLIST ConvertPathToLpItemIdList(OLECHAR *pszPath)
{
	LPITEMIDLIST  pidl = NULL;
	LPSHELLFOLDER pDesktopFolder = NULL;
	ULONG         chEaten;
	HRESULT       hr;

	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		hr = pDesktopFolder->ParseDisplayName(NULL, NULL, 
					pszPath, &chEaten, &pidl, NULL);
		pDesktopFolder->Release();
		return pidl;
	}
	return NULL;
}

bool BrowseFolder( WCHAR* lpRootDir, CString& csPath_o )
{

	bool retVal = false;
	 HWND hOwner = NULL;
	 std::string folderpath;
   // The BROWSEINFO struct tells the shell 
   // how it should display the dialog.
   BROWSEINFO bi;
   memset(&bi, 0, sizeof(bi));

   bi.pidlRoot = ConvertPathToLpItemIdList( lpRootDir );
   bi.ulFlags   = BIF_NONEWFOLDERBUTTON  | BIF_RETURNONLYFSDIRS  | BIF_USENEWUI | BIF_NEWDIALOGSTYLE ;
   bi.hwndOwner = hOwner;
   bi.lpszTitle = L"Choose a base directory";

   // must call this if using BIF_USENEWUI
   ::OleInitialize(NULL);

   // Show the dialog and get the itemIDList for the 
   // selected folder.
   LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

   if(pIDL != NULL)
   {
      // Create a buffer to store the path, then 
      // get the path.
      TCHAR buffer[_MAX_PATH] = {'\0'};
      if(::SHGetPathFromIDList(pIDL, buffer) != 0)
	  {
		  csPath_o = buffer;
		  csPath_o.TrimRight( L"\\");
		  retVal = true;
	  }
      // free the item id list
      CoTaskMemFree(pIDL);
   }
   ::OleUninitialize();
   return  retVal;
}



void CSettings::OnNewBaseDirectory()
{
	std::string folderpath;
	CString csPath;
	if( BrowseFolder( L"", csPath ))
	{
		ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
		std::wstring wsPath(csPath);

		BaseDirectory base;
		base.SetPath(wsPath);

		pConfig->SetBaseDirectory(base);
		UpdateCombos();
	}		
}


void CSettings::OnReloadRepository()
{
	CString str;
	// mCombRespository.GetWindowText(str);

	int nIndx = mCombRespository.GetCurSel();
	if( -1 == nIndx )
	{
		MessageBox( L"Select a repository", L"Quick Launch", MB_OK | MB_ICONINFORMATION);
		return;
	}
	mCombRespository.GetLBText( nIndx, str );
	std::wstring wsRepositoryName (str);


	nIndx = mCombBaseDir.GetCurSel();
	if( -1 == nIndx )
	{
		MessageBox( L"Select a base directory", L"Quick Launch", MB_OK | MB_ICONINFORMATION);
		return;
	}
	mCombBaseDir.GetLBText( nIndx, str );
	std::wstring wsBaseDirectory (str);

	// Set the selected items
	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
	RepositoryName repoName;	
	repoName.SetName(wsRepositoryName);	
	pConfig->SetRepositoryFilename(repoName);

	BaseDirectory base;
	base.SetPath(wsBaseDirectory);	
	pConfig->SetBaseDirectory(base);

	DevAssistFactory& obj = DevAssistFactory::Instance();
	SerialStrategyBase* pStrat = obj.GetSerializationStrategy();
	RepositoryBase* pRep = obj.GetRepository();
	pRep->Deserialize( pStrat );


	CDialog::OnOK();
}

void CSettings::UpdateCombos()
{
	mCombRespository.ResetContent();
	mCombBaseDir.ResetContent();
	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();

	std::vector<RepositoryName> lstReporitories = 
	pConfig->GetAllRespositories( );
	for (std::vector<RepositoryName>::iterator it = lstReporitories.begin() ; it != lstReporitories.end(); ++it)
	{
		mCombRespository.InsertString( -1, (*it).GetName().c_str());
	}

	int nHighLight = mCombRespository.FindStringExact( -1, pConfig->GetCurrentRepository().GetName().c_str());
	//if( -1 != nHighLight )
	//{
	//	mCombRespository.SetCurSel( nHighLight  );
	//}
	mCombRespository.SetCurSel( nHighLight  );


	std::vector<BaseDirectory> lstBase = 
	pConfig->GetPrevBaseDirectories( );
	for (std::vector<BaseDirectory>::iterator it = lstBase.begin() ; it != lstBase.end(); ++it)
	{
		mCombBaseDir.InsertString(-1,(*it).GetPath().c_str());
	}
	const std::wstring& Path = pConfig->GetBaseDirectory().GetPath();
	if( 0 != Path.size())
	{
		int nInfc = mCombBaseDir.InsertString( -1, Path.c_str());
		mCombBaseDir.SetCurSel( nInfc );
	}
}
BOOL CSettings::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hIcon = ::LoadIcon( AfxGetInstanceHandle(),  MAKEINTRESOURCE(3004) );
	SetIcon(  hIcon, FALSE );

	CoInitialize(0);
	// CEdit *pCBEdit = 0;//(CEdit*)mCombBaseDir.GetDlgItem( 1001 );
	//pCBEdit->SetReadOnly( true );
	//SHAutoComplete(pCBEdit->m_hWnd, SHACF_FILESYSTEM);

	// pCBEdit =  (CEdit*)mCombRespository.GetDlgItem( 1001 );
	//SHAutoComplete(pCBEdit->m_hWnd, SHACF_FILESYSTEM);
	UpdateCombos();


	// mCombBaseDir.InsertString(-1, pConfig->GetBaseDirectory().c_str());

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

	//LPCTSTR DEVASSIST_REPOSITORY  = _T( "\\REPOSITORY\\" );
	//LPCTSTR DEVASSIST_PROPERTYFILENAME  = _T( "\\Settings.prop" );
	//LPCTSTR DEVASSIST_MASTER_SETTINGS  = _T( "\\master.prop" );
#include "RepDetails.h"
void CSettings::OnBnClickedOpnSettings()
{
	CString str;
	int nIndx = mCombRespository.GetCurSel();
	if( -1 == nIndx )
	{
		MessageBox( L"Select a repository", L"Quick Launch", MB_OK | MB_ICONINFORMATION);
		return;
	}
	mCombRespository.GetLBText( nIndx, str );

	CRepDetails obj( true, str );
	obj.DoModal();

	// ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
	// std::wstring wsInstalledPath = pConfig->GetInstalledPath();
	// CString cs(wsInstalledPath.c_str());
	// cs += DEVASSIST_REPOSITORY;
	// cs += pConfig->GetCurrentRepository().GetName().c_str();
	// cs += DEVASSIST_PROPERTYFILENAME;
	// cs.TrimRight( L"\\");
	//cs += L"\\DevAssistConfig.ini";

	// ShellExecute( 0, L"open", cs, 0, 0,SW_SHOW );
}

void CSettings::OnBnClickedUpdateRepository()
{

	CString strtext;
	int nIndx = mCombRespository.GetCurSel();
	if( -1 == nIndx )
	{
		MessageBox( L"Select a repository", L"Quick Launch", MB_OK | MB_ICONINFORMATION);
		return;
	}
	mCombRespository.GetLBText( nIndx, strtext );
	std::wstring wsRepositoryName (strtext);


	nIndx = mCombBaseDir.GetCurSel();
	if( -1 == nIndx )
	{
		MessageBox( L"Select a base directory", L"Quick Launch", MB_OK | MB_ICONINFORMATION);
		return;
	}
	mCombBaseDir.GetLBText( nIndx, strtext );
	std::wstring wsBaseDirectory (strtext);

	///////////////////////////////////////////////////////////
	
	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
	
	
	RepositoryName repo;
	repo.SetName( wsRepositoryName );

	RepositorySettings settings;
	if( !pConfig->GetRepositorySettings( repo, settings ))
	{
		MessageBox( L"Failed to get repository settings.", L"Quick Launch", MB_OK | MB_ICONERROR);
		return;
	}

	BaseDirectory base;
	base.SetPath( wsBaseDirectory  );

	const std::vector<std::wstring>& vecSubFolders = settings.GetConfiguredFolders();
	CString csInfo;

	CString csFolders(wsBaseDirectory.c_str());
	csFolders.TrimLeft(L"\\");
	csFolders.TrimRight(L"\\");
	csFolders += L"\\\r\n";
	if( vecSubFolders.size() > 0)
	{
		// CString csFolders;
		csFolders = L"";
		for (std::vector<std::wstring>::const_iterator it = vecSubFolders.begin() ; it != vecSubFolders.end(); ++it)
		{
			CString csTemp(wsBaseDirectory.c_str());
			csTemp.TrimLeft(L"\\");
			csTemp.TrimRight(L"\\");
			csTemp += L"\\";
			csTemp += (*it).c_str();
			csTemp += L"\r\n";

			csFolders += csTemp;
		}
	}

	csInfo.Format( L"<%s> Will be indexed from\r\n\r\n%s\r\nWould you like to continue. ?", 
		CString(wsRepositoryName.c_str()), 
		csFolders);

	if( IDCANCEL  == ::MessageBox( this->m_hWnd, csInfo, L"Quick Assist", MB_OKCANCEL | MB_ICONQUESTION ))
	{
		return;
	}
	
	pConfig->SetBaseDirectory(base );
	pConfig->SetRepositoryFilename( repo );

	//////////////
	std::wstring wsInstalledPath = pConfig->GetInstalledPath();
	CString cs(wsInstalledPath.c_str());
	cs.TrimRight( L"\\");
	cs += L"\\FileIndexer.exe";

	::MessageBox( this->m_hWnd, 
				  L"Indexing will be started. Reload the repository when indexing done to reflect the changes.", 
				  L"Quick Assist", MB_OK | MB_ICONINFORMATION );

	ShellExecute( 0, L"open", cs, 0, 0,SW_SHOW );
	CDialog::OnOK();
}


void CSettings::OnCreateNewRepository()
{
	CRepDetails obj( false, L"" );
	obj.DoModal();

	UpdateCombos();
}


void CSettings::OnBnClickedButton6()
{
	ShellExecute ( NULL, L"open", L"mailto:oadivin@gmail.com", NULL, NULL, SW_SHOWNORMAL); 
}
