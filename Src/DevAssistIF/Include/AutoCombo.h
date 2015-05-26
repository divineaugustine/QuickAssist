#pragma once


// AutoCombo

class AutoCombo : public CComboBox
{
	DECLARE_DYNAMIC(AutoCombo)

public:
	AutoCombo();
	virtual ~AutoCombo();

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEditUpdate();

private:

	  CEdit      m_edit;
      CListBox   m_listbox;
	  BOOL m_bAutoComplete;
};


