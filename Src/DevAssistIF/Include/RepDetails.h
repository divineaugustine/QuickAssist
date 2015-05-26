#pragma once

#include "..\Resource.h"
#include "afxwin.h"
#include <Core\ConfigI.h>

// CRepDetails dialog

class CRepDetails : public CDialogEx
{
	DECLARE_DYNAMIC(CRepDetails)

public:
	CRepDetails(bool bEditExisting, CString csName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRepDetails();

// Dialog Data
	enum { IDD = IDD_REPOSETTINGS };
private:
	bool mbEditExisting;
	CString mcsName;
	RepositoryName mName;
	RepositorySettings mSettings;

	void UpdateText();

	CString mcsExt;
	CString mcsIgnores;
	CString mcsSub;
	int mnFirstSel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox mcmbSetting;
	afx_msg void OnSelchangeComboSetting();
	afx_msg void OnBnClickedBtCreate();
	afx_msg void OnBnClickedBtupdate();
};
