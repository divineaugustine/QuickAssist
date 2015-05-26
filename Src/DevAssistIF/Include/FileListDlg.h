#pragma once
#include "..\Resource.h"
#include "afxwin.h"

// FileListDlg dialog
#include <Core\DataStructs.h>
#include "afxcmn.h"

struct strIcondetails
{
	HICON hIcon;
	int nImgListInfx;
};

class CIconHandler
{
public:
	CIconHandler( CImageList* pList );
	int GetIconIndex( LPCTSTR lpName );
	~CIconHandler();

private:
	CIconHandler();
	void Clear();

	CIconHandler( const CIconHandler& ss);
	CIconHandler& operator=( const CIconHandler& ss);
	CImageList* mpList;
	CMap<CString,LPCTSTR,strIcondetails,strIcondetails> mMap;
};

class MyCrieteria : public ISearchCrieteria<FileInfo>
{
public:

	bool Validate( const FileInfo* pContent_i ) ;
	MyCrieteria();
	~MyCrieteria();
	void SetSearchContent( CString csInput );

private:
CString mcsSearchContent;
CStringList mlstSearchContents;
bool mbMultiple;
};


class FileListDlg : public CDialogEx, public IResultContainer<FileInfo>
{
	DECLARE_DYNAMIC(FileListDlg)

public:
	FileListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~FileListDlg();

	bool AddResult( const FileInfo* pInfo );
	void NotifySearchEnd();
	void setSearch(LPCTSTR lpSearch)
	{
		mcsSearch = lpSearch;
	}

	FileInfo* GetSelectedFile()
	{
		return mpInfo;
	}
// Dialog Data
	enum { IDD = IDD_FileListDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL OnInitDialog();

private:

	ISearch<FileInfo>* mpSearchAssist;
	MyCrieteria mobjCrieteria;
	CString mcsSearch;
	FileInfo* mpInfo;
	CListCtrl mlstCtrlResult;
	CImageList mlstImages;
	CIconHandler mIconHandler;

	CArray<const FileInfo*> mFileInfoDB;
	CCriticalSection mobjDBCritic;
	int AddItem(const FileInfo* pItem);
	const FileInfo* GetItem(int nIndex );
	void ClearAll();

	void ResizeWindow( int nWidth, int nHeight );
	long mlStyle;

	void CollapseList();
	void GrowList();
public:
	// CListBox mLstresult;
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit mEdtsearchText;
	afx_msg void OnSearchData();
	afx_msg void OnClose();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnGetdispinfoList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOdfinditemList2(NMHDR *pNMHDR, LRESULT *pResult);
};
