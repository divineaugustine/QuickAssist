#pragma once
#include "afxwin.h"
#include "..\resource.h"

#include <Core\DataStructs.h>

// CDirectoryDlg dialog

class CDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDirectoryDlg)

public:
	CDirectoryDlg(std::shared_ptr<DirectoryIterator> iter , CString csFileName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDirectoryDlg();

	DirectoryInfo* GetDirectory(){ return mpDir;}
// Dialog Data
	enum { IDD = IDD_Folders };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox mlstFolders;
	afx_msg void OnDblclkList1();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
private:
	std::shared_ptr<DirectoryIterator> miter;
	DirectoryInfo* mpDir;
	CString mcsFileName;
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
