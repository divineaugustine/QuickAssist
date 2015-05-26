// DevAssistCOM.cpp : Implementation of CDevAssistCOM

#include "stdafx.h"
#include "DevAssistCOM.h"
 //   #include "DevAssistIF_i.c"

// CDevAssistCOM

#include "Bridge\BridgeIF.h"

STDMETHODIMP CDevAssistCOM::ShowSettingsDlg(LONG* lStatus_o)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	*lStatus_o = OpenSettings();
	// TODO: Add your implementation code here
	return S_OK;
}


STDMETHODIMP CDevAssistCOM::GetFilePath(BSTR pFileName, BSTR* lFilePath_o, LONG* lStatus_o)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR FilePath[MAX_PATH];
	*lStatus_o = SearchFile( pFileName, MAX_PATH, FilePath );
	if( BRIDGE_SUCCESS == *lStatus_o )
	{
		*lFilePath_o = SysAllocString( FilePath );
	}
	return S_OK;
}


STDMETHODIMP CDevAssistCOM::ToggleFile(BSTR strCurrentFile, BSTR* Path_o, LONG* lStatus_o)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your implementation code here

	TCHAR FilePath[MAX_PATH];
	*lStatus_o = ToggleSoruce( strCurrentFile, MAX_PATH, FilePath );
	if( BRIDGE_SUCCESS == *lStatus_o )
	{
		*Path_o = SysAllocString( FilePath );
	}
	return S_OK;
}
