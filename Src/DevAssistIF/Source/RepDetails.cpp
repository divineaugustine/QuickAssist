// RepDetails.cpp : implementation file
//

#include "stdafx.h"
#include "DevAssistIF.h"
#include "RepDetails.h"
#include "afxdialogex.h"


// CRepDetails dialog

IMPLEMENT_DYNAMIC(CRepDetails, CDialogEx)

	CRepDetails::CRepDetails(bool bEditExisting, CString csName, CWnd* pParent /*=NULL*/)
	: CDialogEx(CRepDetails::IDD, pParent), mbEditExisting(bEditExisting),mcsName(csName)
{

}

CRepDetails::~CRepDetails()
{
}

void CRepDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SETTING, mcmbSetting);
}


BEGIN_MESSAGE_MAP(CRepDetails, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SETTING, &CRepDetails::OnSelchangeComboSetting)
	ON_BN_CLICKED(IDC_BT_CREATE, &CRepDetails::OnBnClickedBtCreate)
	ON_BN_CLICKED(IDC_BTUPDATE, &CRepDetails::OnBnClickedBtupdate)
END_MESSAGE_MAP()


// CRepDetails message handlers
#include <Core\DevAssistFactory.h>
void GetString( const std::vector<std::wstring>& vec_i, CString& csOut,CString csDelimiter, bool bReplacePath )
{
	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
	const BaseDirectory& base = pConfig->GetBaseDirectory();

	for (std::vector<std::wstring>::const_iterator it = vec_i.begin() ; it != vec_i.end(); ++it)
	{
		csOut += (*it).c_str();
		csOut += csDelimiter;
	}
	csOut.TrimRight(csDelimiter);

	if( bReplacePath )
	{
		csOut.Replace(L"?", base.GetPath().c_str());
	}
}

void ParseString( CString csOut,CString csDelimiter, std::vector<std::wstring>& vec_o, bool ReplacePath )
{
	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
	const BaseDirectory& base = pConfig->GetBaseDirectory();

	std::wstring wsInput(csOut);
	std::wstring wsDelim(csDelimiter);
	wchar_t* Input = const_cast<wchar_t*>(wsInput.c_str());
	wchar_t* token = std::wcstok(Input, wsDelim.c_str());
	while (token) 
	{
		CString csTemp(token);
		if(ReplacePath)
		{
			if( 0 ==  csTemp.Find( base.GetPath().c_str()) )
			{
				csTemp.Replace(base.GetPath().c_str(), L"?");
			}
			else
			{
				CString csInfo ( L"Make sure folder is under <Base folder>\nIgnoring - \n");
				csInfo += csTemp;
				::MessageBox(0, csInfo, L"Quick Assist", MB_OK | MB_ICONINFORMATION );
				token = std::wcstok(nullptr, wsDelim.c_str());		
				continue;
			}
		}
		std::wstring ws(csTemp);
		vec_o.push_back( ws );
		token = std::wcstok(nullptr, wsDelim.c_str());			
	}
}


void CRepDetails::UpdateText()
{
	if( 0 == mnFirstSel )
	{
		GetDlgItemText(  IDC_EDIT_PROP, mcsExt );
	}
	else if( 1 == mnFirstSel )
	{
		GetDlgItemText(  IDC_EDIT_PROP, mcsIgnores );
	}
	else if( 2 == mnFirstSel )
	{
		GetDlgItemText(  IDC_EDIT_PROP, mcsSub );
	}
	GetDlgItem( IDC_CURR_PATH )->ShowWindow( false );
	int nIndx= mcmbSetting.GetCurSel();
	switch(nIndx)
	{
	case 0: // Extensions
		{
			SetDlgItemText( IDC_EDIT_PROP, mcsExt );
			break;
		}
	case 1:
		{
			SetDlgItemText( IDC_EDIT_PROP, mcsIgnores );
			break;
		}
	case 2:
		{
			SetDlgItemText( IDC_EDIT_PROP, mcsSub );
			// GetDlgItem( IDC_CURR_PATH )->ShowWindow( true );
			break;
		}
	}
	mnFirstSel = nIndx;
}


BOOL CRepDetails::OnInitDialog()
{
	CDialogEx::OnInitDialog();

		HICON hIcon = ::LoadIcon( AfxGetInstanceHandle(),  MAKEINTRESOURCE(3004) );
	SetIcon(  hIcon, FALSE );

	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();

	const BaseDirectory& base = pConfig->GetBaseDirectory();

	CString csbaseIfno;
	csbaseIfno.Format( L"Base Dir : %s", CString(base.GetPath().c_str()));
	SetDlgItemText(IDC_CURR_PATH, csbaseIfno );
	GetDlgItem( IDC_CURR_PATH )->ShowWindow( false );

	if(mbEditExisting)
	{
		SetDlgItemText(IDC_BT_CREATE, L"Delete");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
		pEdit->SetReadOnly();

		SetWindowText( L"Edit Settings");
	}
	else
	{
		SetDlgItemText(IDC_BT_CREATE, L"Create");

		SetWindowText( L"Create new repository");
	}


	SetDlgItemText(IDC_EDIT_NAME, mcsName );
	mcmbSetting.EnableWindow(mbEditExisting);
	GetDlgItem(IDC_EDIT_PROP)->EnableWindow(mbEditExisting);
	GetDlgItem(IDC_BTUPDATE)->EnableWindow(mbEditExisting);	
	

	if( mbEditExisting )
	{
		std::wstring wsName(mcsName);
		mName.SetName(wsName);
		 
		if( pConfig->GetCurrentRepository() == mName )
		{
			GetDlgItem(IDC_BT_CREATE)->EnableWindow( false );
		}

		 if( pConfig->GetRepositorySettings( mName, mSettings ) )
		 {
			 const std::vector<std::wstring>& Ext = mSettings.GetSupportedExtensions();
			 GetString( Ext, mcsExt,L"\r\n",false );

			 const std::vector<std::wstring>& sub = mSettings.GetConfiguredFolders();
			 GetString( sub, mcsSub,L"\r\n",false );

			 const std::vector<std::wstring>& ignore = mSettings.GetIgnoreDirectories();
			 GetString( ignore, mcsIgnores,L"\r\n",false );
		 }
	}
	mcmbSetting.AddString(L"Extensions");
	mcmbSetting.AddString(L"Ignore directories");
	mcmbSetting.AddString(L"Specific sub folders");

	mcmbSetting.SetCurSel(0);
	mnFirstSel = 0;
	SetDlgItemText( IDC_EDIT_PROP, mcsExt );
	// UpdateText();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


	// ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
	// std::wstring wsInstalledPath = pConfig->GetInstalledPath();
	// CString cs(wsInstalledPath.c_str());
	// cs += DEVASSIST_REPOSITORY;
	// cs += pConfig->GetCurrentRepository().GetName().c_str();
	// cs += DEVASSIST_PROPERTYFILENAME;
	// cs.TrimRight( L"\\");
	//cs += L"\\DevAssistConfig.ini";



void CRepDetails::OnSelchangeComboSetting()
{
	UpdateText();
}


void CRepDetails::OnBnClickedBtCreate()
{
	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();


	// Delete
	if( mbEditExisting )
	{
		if( pConfig->DeleteRepository( mName ))
		{
			MessageBox( L"Removed", L"Quick Launch", MB_OK | MB_ICONINFORMATION );
		}
		else
		{
			MessageBox( L"Failed to delete", L"Quick Launch", MB_OK | MB_ICONERROR );
		}
		CDialog::OnOK();
		return ;
	}
	// Create new
	else 
	{
		GetDlgItemText(IDC_EDIT_NAME, mcsName );
		std::wstring ws(mcsName);
		mName.SetName( ws );
		if( !pConfig->CreateRepository( mName,mSettings ) )
		{
			MessageBox( L"Failed to create new repository", L"Quick Launch", MB_OK | MB_ICONERROR );
			return;
		}
		const std::vector<std::wstring>& Ext = mSettings.GetSupportedExtensions();
		GetString( Ext, mcsExt,L"\r\n",false );

		const std::vector<std::wstring>& sub = mSettings.GetConfiguredFolders();
		GetString( sub, mcsSub,L"\r\n",false );

		const std::vector<std::wstring>& ignore = mSettings.GetIgnoreDirectories();
		GetString( ignore, mcsIgnores,L"\r\n",false );

		mcmbSetting.SetCurSel(0);
		mnFirstSel = 0;
		SetDlgItemText( IDC_EDIT_PROP, mcsExt );

		SetDlgItemText(IDC_BT_CREATE, L"Delete");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NAME);
		pEdit->SetReadOnly();

		mbEditExisting = true;

		mcmbSetting.EnableWindow(mbEditExisting);
		GetDlgItem(IDC_EDIT_PROP)->EnableWindow(mbEditExisting);
		GetDlgItem(IDC_BTUPDATE)->EnableWindow(mbEditExisting);	

	}
}


void CRepDetails::OnBnClickedBtupdate()
{
	UpdateText();

	std::vector<std::wstring> Ext;
	CString cstemp(mcsExt);
	ParseString( cstemp, L"\r\n",Ext,false  );

	std::vector<std::wstring> sub;
	cstemp = mcsSub;
	ParseString( cstemp, L"\r\n",sub,false  );

	std::vector<std::wstring> ignore;
	 cstemp = mcsIgnores;
	ParseString( cstemp ,L"\r\n", ignore ,false );

	mSettings.SetIgnoreDirectories( ignore );
	mSettings.SetSupportedExtensions(Ext);
	mSettings.SetConfiguredFolders(sub);

	ConfigI* pConfig	= DevAssistFactory::Instance().GetConfig();
	pConfig->SetRepositorySettings( mName, &mSettings );
}
