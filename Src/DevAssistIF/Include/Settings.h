#pragma once

#include "..\Resource.h"
#include "afxwin.h"
#include "AutoCombo.h"
// CSettings dialog

class CSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CSettings)

public:
	CSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettings();

// Dialog Data
	enum { IDD = IDD_SETTINGS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnReloadRepository();
	virtual BOOL OnInitDialog();
	AutoCombo mCombBaseDir;
	AutoCombo mCombRespository;
	afx_msg void OnBnClickedOpnSettings();
	afx_msg void OnBnClickedUpdateRepository();
	afx_msg void OnNewBaseDirectory();
	void UpdateCombos();
	afx_msg void OnCreateNewRepository();
	afx_msg void OnClickedMailTo();
	afx_msg void OnBnClickedButton6();
};
