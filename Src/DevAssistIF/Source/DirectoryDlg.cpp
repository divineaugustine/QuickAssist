// DirectoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DevAssistIF.h"
#include "DirectoryDlg.h"
#include "afxdialogex.h"

#include <Core\DevAssistFactory.h>
#include <Core\ConfigI.h>

// CDirectoryDlg dialog

IMPLEMENT_DYNAMIC(CDirectoryDlg, CDialogEx)

CDirectoryDlg::CDirectoryDlg(std::shared_ptr<DirectoryIterator> iter, CString csFileName ,  CWnd* pParent /*=NULL*/)
: CDialogEx(CDirectoryDlg::IDD, pParent), miter(iter), mcsFileName( csFileName )
{
	mpDir = 0;
}

CDirectoryDlg::~CDirectoryDlg()
{
}

void CDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mlstFolders);
}


BEGIN_MESSAGE_MAP(CDirectoryDlg, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &CDirectoryDlg::OnDblclkList1)
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CDirectoryDlg message handlers


void CDirectoryDlg::OnDblclkList1()
{
	int row = mlstFolders.GetCurSel();
	if(row < 0)
		return;

	mpDir = (DirectoryInfo*)mlstFolders.GetItemData(row);
	OnOK();
}


void CDirectoryDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

		  int nWidth = (pRect->right) - (pRect->left);
	  int nHeight = pRect->bottom - pRect->top;

	// TODO: Add your message handler code here
	// mLstresult.MoveWindow( mLstresult.getwin
	CRect pos;

	mlstFolders.GetWindowRect(pos);
	ScreenToClient(&pos);
	UINT flags = SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE;
	mlstFolders.SetWindowPos(NULL, 0, 0, nWidth-20,nHeight-20, flags);

	// TODO: Add your message handler code here
}


BOOL CDirectoryDlg::OnInitDialog()
{
	ConfigI* pConfig = DevAssistFactory::Instance().GetConfig();

	CDialogEx::OnInitDialog();

		HICON hImage = ::LoadIcon( AfxGetInstanceHandle(),  MAKEINTRESOURCE(3002) );
	SetIcon(  hImage, FALSE );

	while(miter->HasNext())
	{
		DirectoryInfo* pDir = miter->Next();

		std::wstring wsFileOut; // (pDir->GetPath().c_str());

		BaseDirectory base = pConfig->GetBaseDirectory();
		TCHAR tcsz[2048];
		base.ResolveVirtualPath( pDir, tcsz );
		wsFileOut = tcsz;

		CString csFile(wsFileOut.c_str());
		csFile += mcsFileName;

		int nIndx = mlstFolders.AddString( csFile );
		mlstFolders.SetItemData( nIndx, (DWORD_PTR)pDir );
	}
	// mlstFolders.SetFocus();
	mlstFolders.SetSel( 1 );

	int nHeight = (mlstFolders.GetCount()*20) + 30;

	CRect rect;
	GetWindowRect( &rect);
	SetWindowPos(NULL, 0, 0, rect.Width(), nHeight+20, SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE );
	mlstFolders.SetWindowPos(NULL, 0, 0, rect.Width()-20,nHeight-20, SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDirectoryDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
    {
        if( pMsg->wParam==VK_UP || pMsg->wParam==VK_DOWN)
        {
			// Give the arrow down and up of dialog to list box
			// if( pMsg->hwnd != mlstFolders.m_hWnd )
			{
				mlstFolders.SetFocus();
				mlstFolders.SendMessage(WM_KEYDOWN, pMsg->wParam );	
			}
            return TRUE;
        }
			 
        else if(pMsg->wParam==VK_RETURN)
        {
			if( pMsg->hwnd == mlstFolders.m_hWnd )
			{
				OnDblclkList1();
			}
            return TRUE;
        }
		// ESC
        else if(pMsg->wParam== 27 )
        {
            OnCancel();
            return TRUE;
        }
    }
	return CDialogEx::PreTranslateMessage(pMsg);
}
