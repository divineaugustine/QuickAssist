// AutoCombo.cpp : implementation file
//

#include "stdafx.h"
#include "DevAssistIF.h"
#include "AutoCombo.h"


// AutoCombo

IMPLEMENT_DYNAMIC(AutoCombo, CComboBox)

AutoCombo::AutoCombo()
{

}

AutoCombo::~AutoCombo()
{
}


BEGIN_MESSAGE_MAP(AutoCombo, CComboBox)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
END_MESSAGE_MAP()



// AutoCombo message handlers
BOOL AutoCombo::PreTranslateMessage(MSG* pMsg)
{
	// Need to check for backspace/delete. These will modify the text in
     // the edit box, causing the auto complete to just add back the text
     // the user has just tried to delete. 
 
     if (pMsg->message == WM_KEYDOWN)
     {
         m_bAutoComplete = TRUE;
 
         int nVirtKey = (int) pMsg->wParam;
         if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
             m_bAutoComplete = FALSE;
     }
 
     return CComboBox::PreTranslateMessage(pMsg);
}


void AutoCombo::OnEditUpdate() 
  {
    // if we are not to auto update the text, get outta here
    if (!m_bAutoComplete) 
        return;
  
    // Get the text in the edit box
    CString str;
    GetWindowText(str);
    int nLength = str.GetLength();
    
    // Currently selected range
    DWORD dwCurSel = GetEditSel();
    WORD dStart = LOWORD(dwCurSel);
    WORD dEnd   = HIWORD(dwCurSel);
  
    // Search for, and select in, and string in the combo box that is prefixed
    // by the text in the edit box
    if (SelectString(-1, str) == CB_ERR)
    {
        SetWindowText(str);       // No text selected, so restore what was there before
        if (dwCurSel != CB_ERR)
         SetEditSel(dStart, dEnd);   //restore cursor postion
   }
 
   // Set the text selection as the additional text that we have added
   if (dEnd < nLength && dwCurSel != CB_ERR)
   {
       SetEditSel(dStart, dEnd);
   }
   else
   {
       SetEditSel(nLength, -1);
   }
 }