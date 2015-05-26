// FileListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DevAssistIF.h"
#include "FileListDlg.h"
#include "afxdialogex.h"



	CIconHandler::CIconHandler( CImageList* pList ):mpList(pList)
	{
	}

	int CIconHandler::GetIconIndex( LPCTSTR lpName )
	{
		CString csFileName(lpName);
		TCHAR tcsz[MAX_PATH];
		lstrcpy( tcsz, csFileName );
		PathRemoveExtension(tcsz);
		csFileName.Replace(tcsz, L"NameDoesntMatter");

		strIcondetails icon;
		if( mMap.Lookup( csFileName, icon ))
		{
			return icon.nImgListInfx;
		}

		// http://stackoverflow.com/questions/524137/get-icons-for-common-file-types
		SHFILEINFO  shfi;
		// Load a Shell Small icon image
		SHGetFileInfo( csFileName, FILE_ATTRIBUTE_NORMAL, &shfi, sizeof(SHFILEINFO), 
			SHGFI_USEFILEATTRIBUTES 
			| SHGFI_ICON | SHGFI_SHELLICONSIZE | SHGFI_SMALLICON);

		icon.hIcon= shfi.hIcon;
		icon.nImgListInfx = mpList->Add( icon.hIcon );
		mMap.SetAt(csFileName, icon );
		return icon.nImgListInfx ;
	}

	CIconHandler::~CIconHandler()
	{
		Clear();
	}



	void CIconHandler::Clear()
	{
		POSITION pos = mMap.GetStartPosition();
		while( pos )
		{
			strIcondetails icon;
			CString csKey;
			mMap.GetNextAssoc( pos, csKey, icon );
			DestroyIcon( icon.hIcon );
		}
		mMap.RemoveAll();
	}

	bool MyCrieteria::Validate( const FileInfo* pContent_i ) 
	{
		if( mcsSearchContent.IsEmpty())
		{
			return true;
		}
		CString csFile( pContent_i->GetFileName().c_str());
		csFile.MakeUpper();
		// _tprintf( L"%s\n", csFile);

		if( mbMultiple )
		{
			POSITION pos = mlstSearchContents.GetHeadPosition();
			while( 0 != pos )
			{
				CString csContent;
				csContent = mlstSearchContents.GetNext( pos );
				if( -1 == csFile.Find(csContent))
				{ 
					return false;
				}
			}
			return true;
		}
		else 
		{
			if( -1 != csFile.Find(mcsSearchContent))
			{ 
				return true;
			}
		}
		return false;
	}

	 MyCrieteria::MyCrieteria() : mbMultiple(false)
	 {}
	 MyCrieteria::~MyCrieteria(){}

	void  MyCrieteria::SetSearchContent( CString csInput )
	{
		mcsSearchContent = csInput;
		mcsSearchContent.MakeUpper();
		mlstSearchContents.RemoveAll();
		CString resToken;
		int curPos = 0;
		mbMultiple = false;
		if( -1 != mcsSearchContent.Find(L"&"))
		{
			resToken= mcsSearchContent.Tokenize(_T("&"),curPos);
			while (resToken != _T(""))
			{
				mlstSearchContents.AddTail( resToken );
				resToken = mcsSearchContent.Tokenize(_T("&"), curPos);
				mbMultiple = true;
			}; 
		}
	}

// FileListDlg dialog

IMPLEMENT_DYNAMIC(FileListDlg, CDialogEx)

FileListDlg::FileListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(FileListDlg::IDD, pParent), 	mIconHandler( &mlstImages )
{
mpInfo = 0;
}

FileListDlg::~FileListDlg()
{
		// CIconHandler::Instance().Clear();
// 	delete mpIconHandler;	
}

void FileListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	// DDX_Control(pDX, IDC_LIST1, mLstresult);
	DDX_Control(pDX, IDC_EDIT1, mEdtsearchText);
	DDX_Control(pDX, IDC_LIST2, mlstCtrlResult);
}

BEGIN_MESSAGE_MAP(FileListDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &FileListDlg::OnLbnSelchangeList1)	
	ON_WM_NCHITTEST()
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT1, &FileListDlg::OnSearchData)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &FileListDlg::OnLvnItemchangedList2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &FileListDlg::OnDblclkList2)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST2, &FileListDlg::OnGetdispinfoList2)
	ON_NOTIFY(LVN_ODFINDITEM, IDC_LIST2, &FileListDlg::OnOdfinditemList2)
END_MESSAGE_MAP()


// FileListDlg message handlers


void FileListDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

LRESULT FileListDlg::OnNcHitTest(CPoint point)
{
    UINT nHit = CDialog::OnNcHitTest(point);
    return (nHit == HTCLIENT ? HTCAPTION : nHit);
	return CDialogEx::OnNcHitTest(point);
}

#include <Core\DevAssistFactory.h>
#include <Core\Repository.h>
#include <Core\ConfigI.h>



void FileListDlg::NotifySearchEnd()
{
	HICON hImage = ::LoadIcon( AfxGetInstanceHandle(),  MAKEINTRESOURCE(3002) );
	SetIcon(  hImage, FALSE );
	
		ConfigI* pConfig = DevAssistFactory::Instance().GetConfig();
	RepositoryName repName =  pConfig->GetCurrentRepository();
	BaseDirectory baseDir = pConfig->GetBaseDirectory();

	CString csFormatCap;
	csFormatCap.Format(L"Search done in <%s>",/*CString(repName.GetName().c_str()),*/ CString(baseDir.GetPath().c_str()));
	SetWindowText( csFormatCap );


	if( 0 == mlstCtrlResult.GetItemCount())
	{
		CollapseList();
	}
}

	int FileListDlg::AddItem(const FileInfo* pItem)
	{
		CSingleLock objLock( &mobjDBCritic );
		objLock.Lock();
		mFileInfoDB.Add( pItem );
		return mFileInfoDB.GetCount();
	}

	const FileInfo* FileListDlg::GetItem(int nIndex )
	{
		CSingleLock objLock( &mobjDBCritic );
		objLock.Lock();
		if( nIndex >= mFileInfoDB.GetCount())
		{
			return 0;
		}
		return mFileInfoDB.GetAt( nIndex );
	}

	void FileListDlg::ClearAll()
	{
		CSingleLock objLock( &mobjDBCritic );
		objLock.Lock();
		mFileInfoDB.RemoveAll();
	}

bool FileListDlg::AddResult( const FileInfo* pInfo )
{
	int nTotalCount = AddItem( pInfo );
	mlstCtrlResult.SetItemCountEx( nTotalCount );
	GrowList();
	return true;
}

void FileListDlg::OnGetdispinfoList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	//Create a pointer to the item
    LV_ITEM* pItem= &(pDispInfo)->item;

    //Which item number?
    int itemid = pItem->iItem;
	if( itemid >= mlstCtrlResult.GetItemCount())
	{
		AfxMessageBox( L"Count mismatch ");
		return;
	}

    //Do the list need text information?
    if (pItem->mask & LVIF_TEXT)
    {
        CString text;

        //Which column?
        if(pItem->iSubItem == 0)
        {
			const FileInfo*  pInfo =  GetItem( itemid );
			if( !pInfo)
			{
				return;
			}
			text = pInfo->GetFileName().c_str();
        }

        //Copy the text to the LV_ITEM structure
        //Maximum number of characters is in pItem->cchTextMax
        lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
    }

    //Do the list need image information?
    if( pItem->mask & LVIF_IMAGE) 
    {
		const FileInfo*  pInfo =  GetItem( itemid );
		if( !pInfo)
		{
			return;
		}
		CString text = pInfo->GetFileName().c_str();
		pItem->iImage = mIconHandler.GetIconIndex( text );
    }
	*pResult = 0;
}

void FileListDlg::OnOdfinditemList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
    // pNMHDR has information about the item we should find
    // In pResult we should save which item that should be selected

    /* pFindInfo->iStart is from which item we should search.
       We search to bottom, and then restart at top and will stop
       at pFindInfo->iStart, unless we find an item that match
     */

    // Set the default return value to -1
    // That means we didn't find any match.
    *pResult = -1;

    //Is search NOT based on string?
    if( (pFindInfo->lvfi.flags & LVFI_STRING) == 0 )
    {
        //This will probably never happend...
        return;
    }

    //This is the string we search for
    CString searchstr = pFindInfo->lvfi.psz;

    int startPos = pFindInfo->iStart;
    //Is startPos outside the list (happens if last item is selected)
    if(startPos >= mlstCtrlResult.GetItemCount())
        startPos = 0;

    int currentPos=startPos;
    
    //Let's search...
    do
    {        
		const FileInfo*  pInfo =  GetItem( currentPos );
		if( !pInfo)
		{
			return;
		}
		CString text = pInfo->GetFileName().c_str();

        //Do this word begins with all characters in searchstr?
        if( _tcsnicmp(text, searchstr, searchstr.GetLength()) == 0)
        {
            //Select this item and stop search.
            *pResult = currentPos;
            break;
        }

        //Go to next item
        currentPos++;

        //Need to restart at top?
        if(currentPos >= mlstCtrlResult.GetItemCount())
            currentPos = 0;

    //Stop if back to start
    }while(currentPos != startPos);
}


int gnWindowSizeDiff = 0;
int gnWindowWidthDiff = 0;


void FileListDlg::ResizeWindow( int nWidth, int nHeight )
{
	CRect pos;
	mEdtsearchText.GetWindowRect(pos);
	ScreenToClient(&pos);
	UINT flags = SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE;
	mEdtsearchText.SetWindowPos(NULL, 0, 0, nWidth-(gnWindowWidthDiff*2), pos.Height(), flags);
	mlstCtrlResult.SetWindowPos(NULL, 0, 0, nWidth-(gnWindowWidthDiff*2), nHeight-pos.Height()-(12+gnWindowSizeDiff+5), flags);
	mlstCtrlResult.SetColumnWidth( 0, nWidth-(gnWindowWidthDiff*4)-10);
}

void FileListDlg::CollapseList()
{
	CRect rect;
	mEdtsearchText.GetWindowRect( &rect);
	UINT flags = SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE;
	SetWindowPos(NULL, 0, 0, rect.Width()+(gnWindowWidthDiff*2), rect.Height()+12+gnWindowSizeDiff, flags);

	GetWindowRect( &rect);
	ResizeWindow(rect.Width(), rect.Height());
}

void FileListDlg::GrowList()
{

	// mlstCtrlResult.ShowWindow( SW_SHOW );
	CRect rect;
	GetWindowRect( &rect);

	// ::SetWindowLong( m_hWnd, GWL_STYLE, mlStyle );

	if( rect.Height() < 250 )
	{
		UINT flags = SWP_NOSENDCHANGING |SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE;
		int nToGrow = 20;
		// The first one
		if( 1 == mlstCtrlResult.GetItemCount())
		{
			nToGrow += 25;
		}
		SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height()+nToGrow, flags);


		GetWindowRect( &rect);
		ResizeWindow(rect.Width(), rect.Height());
	}
}

BOOL FileListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect dlgRect;
	GetWindowRect(&dlgRect );
	
	CRect rectEdit;
	mEdtsearchText.GetWindowRect( &rectEdit );
	gnWindowSizeDiff = rectEdit.top-dlgRect.top;

	ClientToScreen(&dlgRect);
	ClientToScreen(&rectEdit);
		
	gnWindowWidthDiff = rectEdit.left-dlgRect.left;

	// CString csDiff;
	// csDiff.Format(L"height diff = %d : width diff = %d", gnWindowSizeDiff, gnWindowWidthDiff);
	// AfxMessageBox(csDiff);


	mlStyle = GetWindowLong(m_hWnd, GWL_STYLE );

	// TODO:  Add extra initialization here
	HICON hImage = ::LoadIcon( AfxGetInstanceHandle(),  MAKEINTRESOURCE(3002) );
	SetIcon(  hImage, FALSE );

	mlstCtrlResult.InsertColumn(0, L"");

	mlstImages.Create(24,24,ILC_COLOR32,0,0);
	mlstCtrlResult.SetImageList(&mlstImages,LVSIL_SMALL);
	mlstCtrlResult.SetExtendedStyle( /*LVS_SINGLESEL |*/ LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES  /* | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE*/);

	//CRect rect;
	//GetWindowRect( &rect);
	//mlstCtrlResult.SetColumnWidth( 0, rect.Width()-(gnWindowWidthDiff*4));
	
	ConfigI* pConfig = DevAssistFactory::Instance().GetConfig();
	RepositoryName repName =  pConfig->GetCurrentRepository();
	BaseDirectory baseDir = pConfig->GetBaseDirectory();

	CString csFormatCap;
	csFormatCap.Format(L"Search in <%s>",/*CString(repName.GetName().c_str()),*/ CString(baseDir.GetPath().c_str()));
	SetWindowText( csFormatCap );

	CollapseList();
	RepositoryBase* pRepo = DevAssistFactory::Instance().GetRepository();
	std::shared_ptr<FileIterator> pIter = pRepo->CreateFileIterator();	 
	mpSearchAssist = DevAssistFactory::Instance().CreateThreadSearch( pIter, this );

	

	// This initiates search

	if( !mcsSearch.IsEmpty() )
	{
		mEdtsearchText.SetWindowText(mcsSearch);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void FileListDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	__super::OnSizing(fwSide, pRect);

	// mlstCtrlResult.ShowWindow( SW_SHOW );

	int nWidth = (pRect->right) - (pRect->left);
	int nHeight = pRect->bottom - pRect->top;
	ResizeWindow( nWidth, nHeight );
}


void FileListDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}


void FileListDlg::OnSearchData()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	ConfigI* pConfig = DevAssistFactory::Instance().GetConfig();
	RepositoryName repName =  pConfig->GetCurrentRepository();
	BaseDirectory baseDir = pConfig->GetBaseDirectory();

	CString csFormatCap;
	csFormatCap.Format(L"Searching in <%s>",/*CString(repName.GetName().c_str()),*/ CString(baseDir.GetPath().c_str()));
	SetWindowText( csFormatCap );


	CString csSearch;
	mEdtsearchText.GetWindowTextW(csSearch);
	mpSearchAssist->Stop();
	mlstCtrlResult.DeleteAllItems();
	ClearAll();

	// Will introduce a small flickering when we trype
	// CollapseList();

	RepositoryBase* pRepo = DevAssistFactory::Instance().GetRepository();
	std::shared_ptr<FileIterator> pIter = pRepo->CreateFileIterator();
	mobjCrieteria.SetSearchContent(csSearch );
	mpSearchAssist->StartSearch( pIter, &mobjCrieteria );
}


void FileListDlg::OnClose()
{
	mpSearchAssist->Stop();
	__super::OnClose();
}


BOOL FileListDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

////1st and 2nd coloumns are fixed by the following code. 
//
//	if((pHDN->hdr.code == HDN_BEGINTRACKW || pHDN->hdr.code == HDN_BEGINTRACKA)
//		&& ((pHDN->iItem == 0) /*|| pHDN->iItem == 2 */))
//     {
//        *pResult = TRUE;
//        return TRUE;
//      }

	return __super::OnNotify(wParam, lParam, pResult);
}


void FileListDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		*pResult = 0;
	*pResult = 0;
}


void FileListDlg::OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;
	int index = pNMItemActivate ->iItem;
	if( -1 == index )
	{
		return;
	}

	mpInfo =  (FileInfo*)GetItem( index );
	// mpInfo= (FileInfo*)mlstCtrlResult.GetItemData( index );
	mpSearchAssist->Stop();
	OnOK();
}


void FileListDlg::OnCancel()
{
		mpSearchAssist->Stop();
	__super::OnCancel();
}


BOOL FileListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
    {
        if( pMsg->wParam==VK_UP || pMsg->wParam==VK_DOWN)
        {
			// Give the arrow down and up of dialog to list box
			// if( pMsg->hwnd != mlstCtrlResult.m_hWnd )
			{
				mlstCtrlResult.SetFocus();
				mlstCtrlResult.SendMessage(WM_KEYDOWN, pMsg->wParam );
			}
            return TRUE;
        }
		else if(pMsg->wParam==VK_BACK)
		{
			mEdtsearchText.SetFocus();
			mEdtsearchText.SendMessage(WM_KEYDOWN, pMsg->wParam );	
		}
        else if(pMsg->wParam==VK_RETURN)
        {
			// Give the arrow down and up of dialog to list box
			if( pMsg->hwnd == mlstCtrlResult.m_hWnd )
			{
				int index = mlstCtrlResult.GetNextItem(-1, LVNI_SELECTED);
				if( -1 != index ) 
				{
					mpInfo =  (FileInfo*)GetItem( index );
					// mpInfo= (FileInfo*)mlstCtrlResult.GetItemData( index );
					mpSearchAssist->Stop();
					OnOK();
				}
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


	return __super::PreTranslateMessage(pMsg);
}
