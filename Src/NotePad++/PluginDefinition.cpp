//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include "PluginDefinition.h"
#include "menuCmdID.h"
#include "Resource.h"

#include <Bridge\BridgeIF.h>


//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

ShortcutKey myShortCuts[nbFunc];
toolbarIcons myToolBars[nbFunc];
//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;
HINSTANCE ghModule;
//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
	ghModule =(HINSTANCE) hModule;
}

 // #include <Core\DevAssistFactory.h>
 // #include <Core\Repository.h>


void FillToolBar(toolbarIcons* pToolBar, LPCWSTR nResId )
{
	// RepositoryBase* pRepo = DevAssistFactory::Instance().GetRepository();

	pToolBar->hToolbarIcon = 0;
	pToolBar->hToolbarBmp = 
		LoadBitmap(ghModule, nResId );
	if( NULL == pToolBar->hToolbarBmp )
	{
		DWORD dwError = GetLastError();
		TCHAR sz[MAX_PATH];
		wsprintf( sz, L"%d", dwError );
	}
}

void RegisterToolBar()
{
	FillToolBar(&myToolBars[0], MAKEINTRESOURCE(101) );
	FillToolBar(&myToolBars[1], MAKEINTRESOURCE(IDB_FILESEARCH) );
	FillToolBar(&myToolBars[2], MAKEINTRESOURCE(IDB_SETTINGS) );
	FillToolBar(&myToolBars[3], MAKEINTRESOURCE(IDB_CONTEXTMENU) );

	//myToolBars[1].hToolbarIcon = 0;

	//myToolBars[1].hToolbarBmp = 
	//	LoadBitmap(ghModule, MAKEINTRESOURCE(101));
	//if( NULL == myToolBars[1].hToolbarBmp )
	//{
	//	 ::MessageBox(NULL, L"RegisterToolBar", L"", MB_OK);
	//	DWORD dwError = GetLastError();
	//	TCHAR sz[MAX_PATH];
	//	wsprintf( sz, L"%d", dwError );
	//	 ::MessageBox(NULL, sz, L"", MB_OK);
	//}

LRESULT  hl = ::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[0]._cmdID,(LPARAM)&(myToolBars[0]) );
	  hl = ::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[1]._cmdID,(LPARAM)&(myToolBars[1]) );
	  hl = ::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[2]._cmdID,(LPARAM)&(myToolBars[2]) );
	  hl = ::SendMessage(nppData._nppHandle, NPPM_ADDTOOLBARICON, (WPARAM)funcItem[3]._cmdID,(LPARAM)&(myToolBars[3]) );
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

void fillshortcut(ShortcutKey* pStruct, UCHAR key )
{
	pStruct->_isCtrl = true;
	pStruct->_isAlt = false;
	pStruct->_isShift = false;
	pStruct->_key = key;

}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{
	fillshortcut( &myShortCuts[0], 0x60 );
	fillshortcut( &myShortCuts[1], 0x61 );
	fillshortcut( &myShortCuts[2], 0x62 );
	fillshortcut( &myShortCuts[3], 0x63 );


    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
	setCommand(0, TEXT("Toggle"), ToggleFile, &myShortCuts[0], false);	
	setCommand(1, TEXT("Open File"), OpenFile, &myShortCuts[1], false);
	setCommand(2, TEXT("Show Settings"), ShowSettings, &myShortCuts[2], false);
    setCommand(3, TEXT("Explorer Menu"), OpenContextMenu, &myShortCuts[3], false);		
}



//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}
// 
#define STRICT_TYPED_ITEMIDS 

#include <shlobj.h>

//#include <algorithm>
//#include <array>
//#include <iostream>
//#include <functional>

bool openShellContextMenuForObject( LPCWSTR  path, int xPos, int yPos, void * parentWindow)
{
	CoInitialize(0);
	// assert (parentWindow);
	ITEMIDLIST * id = 0;
	PIDLIST_ABSOLUTE stId = NULL; 
	// std::wstring windowsPath = path;
	// std::replace(windowsPath.begin(), windowsPath.end(), '/', '\\');
	HRESULT result = SHParseDisplayName(path, 0, &stId, 0, 0);
	if (!SUCCEEDED(result) || !stId)
		return false;
	// CItemIdListReleaser idReleaser (id);

	IShellFolder * ifolder = 0;

	// LPCITEMIDLIST idChild = 0;
	PCUITEMID_CHILD idChild = 0;
	result = SHBindToParent(stId, IID_IShellFolder, (void**)&ifolder, &idChild);
	if (!SUCCEEDED(result) || !ifolder)
	{
		return false;
	}
	// CComInterfaceReleaser ifolderReleaser (ifolder);

	// LPCONTEXTMENU imenu = NULL;
	 IContextMenu * imenu = 0;
	//IContextMenu3* pHigherMenu = 0;

	result = ifolder->GetUIObjectOf((HWND)parentWindow, 1, (PCUITEMID_CHILD_ARRAY)&idChild, 
		IID_IContextMenu, 0, (void**)&imenu);
	if (!SUCCEEDED(result) || !ifolder)
	{
		return false;
	}
	// CComInterfaceReleaser menuReleaser(imenu);

	///////
	//if (imenu->QueryInterface(IID_IContextMenu3, pHigherMenu) == NOERROR)
	//	imenu->Release();
////////////

	HMENU hMenu = CreatePopupMenu();
	if (!hMenu)
		return false;
	if (SUCCEEDED(imenu->QueryContextMenu(hMenu, 0, 1, 0x7FFF, CMF_NORMAL  /*CMF_EXPLORE*/  )))
	{
		int iCmd = TrackPopupMenuEx(hMenu, TPM_RETURNCMD, xPos, yPos, (HWND)parentWindow, NULL);
		if (iCmd > 0)
		{
			CMINVOKECOMMANDINFOEX info = { 0 };
			info.cbSize = sizeof(info);
			info.fMask = CMIC_MASK_UNICODE;
			info.hwnd = (HWND)parentWindow;
			info.lpVerb  = MAKEINTRESOURCEA(iCmd - 1);
			info.lpVerbW = MAKEINTRESOURCEW(iCmd - 1);
			info.nShow = SW_SHOWNORMAL;
			imenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
		}
	}
	DestroyMenu(hMenu);

	CoUninitialize();
	return true;
}
//
//
/////////////////////http://blogs.msdn.com/b/oldnewthing/archive/2004/09/22/232836.aspx//////
//
//
//#include <Shobjidl.h>
//
//#define SCRATCH_QCM_FIRST 1
//#define SCRATCH_QCM_LAST  0x7FFF
//
//#undef HANDLE_WM_CONTEXTMENU
//#define HANDLE_WM_CONTEXTMENU(hwnd, wParam, lParam, fn) \
//    ((fn)((hwnd), (HWND)(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), 0L)
//
//#include <shlobj.h>
//
//HRESULT GetUIObjectOfFile(HWND hwnd, LPCWSTR pszPath, REFIID riid, void **ppv)
//{
//  *ppv = NULL;
//  HRESULT hr;
//  PIDLIST_ABSOLUTE pidl;
//  SFGAOF sfgao;
//  if (SUCCEEDED(hr = SHParseDisplayName(pszPath, NULL, &pidl, 0, &sfgao))) {
//    IShellFolder *psf;
//    PCUITEMID_CHILD pidlChild;
//    if (SUCCEEDED(hr = SHBindToParent(pidl, IID_IShellFolder,
//                                      (void**)&psf, &pidlChild))) {
//      hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
//      psf->Release();
//    }
//    CoTaskMemFree(pidl);
//  }
//  return hr;
//}
//
//// WARNING! Incomplete and buggy! See discussion
//void OnContextMenu(HWND hwnd, LPCWSTR  path, int xPos, int yPos)
//{
//  //POINT pt = { xPos, yPos };
//  //if (pt.x == -1 && pt.y == -1) {
//  //  pt.x = pt.y = 0;
//  //  ClientToScreen(hwnd, &pt);
//  //}
//
//  IContextMenu *pcm;
//  if (SUCCEEDED(GetUIObjectOfFile(hwnd, path,
//                   IID_IContextMenu, (void**)&pcm)))
//  {
//    HMENU hmenu = CreatePopupMenu();
//    if (hmenu) {
//      if (SUCCEEDED(pcm->QueryContextMenu(hmenu, 0,
//                             SCRATCH_QCM_FIRST, SCRATCH_QCM_LAST,
//                             CMF_NORMAL))) {
//        int iCmd = TrackPopupMenuEx(hmenu, TPM_RETURNCMD,
//                                    xPos, yPos, hwnd, NULL);
//        if (iCmd > 0) {
//          CMINVOKECOMMANDINFOEX info = { 0 };
//          info.cbSize = sizeof(info);
//          info.fMask = CMIC_MASK_UNICODE;
//          info.hwnd = hwnd;
//          info.lpVerb  = MAKEINTRESOURCEA(iCmd - SCRATCH_QCM_FIRST);
//          info.lpVerbW = MAKEINTRESOURCEW(iCmd - SCRATCH_QCM_FIRST);
//          info.nShow = SW_SHOWNORMAL;
//          pcm->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
//        }
//      }
//      DestroyMenu(hmenu);
//    }
//    pcm->Release();
//  }
//}
///////////////////////////////////

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//





class DevAssistAdapter
{
public:

	DevAssistAdapter(){}
	virtual ~DevAssistAdapter(){}
	virtual long ToggleImpl( LPTSTR lpFileName, LPTSTR lpOut, int Length_i ) = 0;
	virtual long OpenFileImpl ( LPTSTR filename, LPTSTR lpOut, int Length_i ) = 0;
	virtual long ShowSettingsImpl() = 0;

private:

	const DevAssistAdapter& operator= (const DevAssistAdapter&  src );
	DevAssistAdapter(const DevAssistAdapter&  src );
};


typedef long (_cdecl  *PToggleSoruce)(LPCWSTR , int , LPWSTR );
typedef long (_cdecl  *POpenSettings)();
typedef long (_cdecl  *PSearchFile)( LPCWSTR , int , LPWSTR );

class DllLoadAdapter: public DevAssistAdapter
{
public:

	DllLoadAdapter();
	virtual ~DllLoadAdapter(){}
	virtual long ToggleImpl( LPTSTR lpFileName, LPTSTR lpOut, int Length_i ) ;
	virtual long OpenFileImpl ( LPTSTR filename, LPTSTR lpOut, int Length_i ) ;
	virtual long ShowSettingsImpl() ;

private:

	void InitDll();
	bool GetInstalledPathFromReg( TCHAR* tcszOut, int nSize );
	const DllLoadAdapter& operator= (const DllLoadAdapter&  src );
	DllLoadAdapter(const DllLoadAdapter&  src );
	
private:

	HMODULE mhInstance ;
	PToggleSoruce mpToggle ;
	POpenSettings mpSettings ;
	PSearchFile mpOpenFile ;

};

	DllLoadAdapter::DllLoadAdapter() : mhInstance(0), mpToggle(0),mpSettings(0),mpOpenFile(0)
	{
		InitDll();
	}

	long DllLoadAdapter::ToggleImpl( LPTSTR lpFileName, LPTSTR lpOut, int Length_i )
	{
		if( mpToggle )
		{
			return mpToggle( lpFileName, Length_i, lpOut );
		}
		return -1;
	}

	long DllLoadAdapter::OpenFileImpl ( LPTSTR filename, LPTSTR lpOut, int Length_i )
	{
		if( mpOpenFile )
		{
			return mpOpenFile( filename, Length_i, lpOut );
		}
		return -1;
	}

	long DllLoadAdapter::ShowSettingsImpl()
	{
		if( mpSettings )
		{
			return mpSettings();
		}
		return -1;
	}

	void DllLoadAdapter::InitDll()
	{
		TCHAR tcsx[MAX_PATH];
		::GetCurrentDirectory( MAX_PATH, tcsx );

		TCHAR tcszInstallPath[MAX_PATH];
		GetInstalledPathFromReg(tcszInstallPath,MAX_PATH);

		::SetCurrentDirectory( tcszInstallPath );
		mhInstance = ::LoadLibrary( L"DevAssistIF.dll");
		::SetCurrentDirectory( tcsx );
		if (  mhInstance )
		{
			mpToggle = (PToggleSoruce)GetProcAddress( mhInstance, "ToggleSoruce");
			mpSettings = (POpenSettings)GetProcAddress( mhInstance, "OpenSettings");
			mpOpenFile = (PSearchFile)GetProcAddress( mhInstance, "SearchFile");
		}
		else
		{
			::MessageBox(0, L"Failed to load the Dll\r\n"
				L"Setup installation path as below\r\n"
				L"HKEY_CURRENT_USER\\SOFTWARE\\DevAssist<InstallDir>", 
				L"Quick Launch",MB_OK | MB_ICONERROR );
		}
	}


	bool DllLoadAdapter::GetInstalledPathFromReg( TCHAR* tcszOut, int nSize )
	{
		LPCTSTR DEVASSIST_REG_ROOT = L"SOFTWARE\\DevAssist";
		LPCTSTR DEVASSIST_INSTALL_KEY =L"InstallDir" ;

		const int ZERO_CVAL = 0;
		DWORD dValLength = nSize-1;
		DWORD dType	     = 0;
		DWORD dwDisp = 0;
		LPDWORD lpdwDisp = &dwDisp;
		// TCHAR szDriveName[MAX_PATH]={'\0'};
		HKEY hKey;
		long lResult = RegOpenKeyEx(HKEY_CURRENT_USER, \
						DEVASSIST_REG_ROOT, \
						ZERO_CVAL, \
						KEY_QUERY_VALUE, \
						&hKey);                
		if(ERROR_SUCCESS != lResult)
		{
			return false;
		}

		lResult = RegQueryValueEx(\
			hKey, \
			DEVASSIST_INSTALL_KEY, \
			NULL, \
			&dType, \
			(BYTE *)tcszOut, \
			&dValLength);

		if((ERROR_SUCCESS != lResult) && (ERROR_FILE_NOT_FOUND != lResult))
		{
			return false;
		}
		else
		{
			return true;
		}
	}



	#include <..\DevAssistIF\DevAssistIF_i.h>
#include <..\DevAssistIF\DevAssistIF_i.c>

class ComAdapter : public DevAssistAdapter
{
public:

	ComAdapter(){}
	virtual ~ComAdapter(){}
	virtual long ToggleImpl( LPTSTR lpFileName, LPTSTR lpOut, int Length_i ) ;
	virtual long OpenFileImpl ( LPTSTR filename, LPTSTR lpOut, int Length_i ) ;
	virtual long ShowSettingsImpl() ;

private:

	const ComAdapter& operator= (const ComAdapter&  src );
	ComAdapter(const ComAdapter&  src );

	void ShowRegistrationMsg();
};




long ComAdapter::ToggleImpl( LPTSTR lpFileName, LPTSTR lpOut, int Length_i )
{
	LONG lReturn = -1;
	CoInitialize(0);

	HRESULT hr;                    //COM error code;
	IDevAssistCOM *chat;    //pointer to the interface

	hr = CoCreateInstance(
		CLSID_DevAssistCOM,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDevAssistCOM,
		(void**) &chat);
	if(SUCCEEDED(hr))
	{
		/*
		**    STEP 4
		** call method via Interface ID.
		*/  


		BSTR Path_o; 
		hr = chat ->ToggleFile( lpFileName, &Path_o, &lReturn );
		if( BRIDGE_SUCCESS == lReturn )
		{
			int which = -1;
			lstrcpy( lpOut, Path_o );
			::SysFreeString(Path_o);
		}
		//for( int nInfx = 0; nInfx < 1024; ++nInfx )
		//{
		//	BSTR message; 
		//	BSTR message1(str.AllocSysString()); 
		//	hr = chat ->GetFilePath( message1, &message, &lReturn );
		//	::SysFreeString(message1);
		//	::SysFreeString(message);
		//}
		// ::MessageBox(NULL, message, L"Message returned from chat server", MB_OK );
		// ::SysFreeString(*message);

		//	hr = chat -> Release();
	}
	else
	{
		ShowRegistrationMsg();
	}

	CoUninitialize();
	return lReturn;
}

long ComAdapter::OpenFileImpl ( LPTSTR filename, LPTSTR lpOut, int Length_i )
{
	CoInitialize(0);
	HRESULT hr;                    //COM error code;
	IDevAssistCOM *chat;    //pointer to the interface
	LONG lReturn = -1;
	hr = CoCreateInstance(
		CLSID_DevAssistCOM,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDevAssistCOM,
		(void**) &chat);
	if(SUCCEEDED(hr))
	{
		/*
		**    STEP 4
		** call method via Interface ID.
		*/  


		BSTR Path_o; 
		hr = chat ->GetFilePath( filename, &Path_o, &lReturn );
		if( BRIDGE_SUCCESS == lReturn )
		{
			int which = -1;
			lstrcpy( lpOut, Path_o );
			::SysFreeString(Path_o);
		}		

		//for( int nInfx = 0; nInfx < 1024; ++nInfx )
		//{
		//	BSTR message; 
		//	BSTR message1(str.AllocSysString()); 
		//	hr = chat ->GetFilePath( message1, &message, &lReturn );
		//	::SysFreeString(message1);
		//	::SysFreeString(message);
		//}
		// ::MessageBox(NULL, message, L"Message returned from chat server", MB_OK );
		// ::SysFreeString(*message);

		//	hr = chat -> Release();
	}
	
	else
	{
		ShowRegistrationMsg();
	}
	CoUninitialize();
	return lReturn;
}

long ComAdapter::ShowSettingsImpl()
{
	CoInitialize(0);
	//used to accept return value from server.
	HRESULT hr;                    //COM error code;
	IDevAssistCOM *chat;    //pointer to the interface
	LONG lReturn = -1;

	hr = CoCreateInstance(
		CLSID_DevAssistCOM,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IDevAssistCOM,
		(void**) &chat);
	if(SUCCEEDED(hr))
	{
		/*
		**    STEP 4
		** call method via Interface ID.
		*/  


		hr = chat ->ShowSettingsDlg( &lReturn );

		//for( int nInfx = 0; nInfx < 1024; ++nInfx )
		//{
		//	BSTR message; 
		//	BSTR message1(str.AllocSysString()); 
		//	hr = chat ->GetFilePath( message1, &message, &lReturn );
		//	::SysFreeString(message1);
		//	::SysFreeString(message);
		//}
		// ::MessageBox(NULL, message, L"Message returned from chat server", MB_OK );
		// ::SysFreeString(*message);

		//	hr = chat -> Release();
	}	
	else
	{
		ShowRegistrationMsg();
	}
	CoUninitialize();
	return lReturn;
}

static DevAssistAdapter* g_pAdapter = 0;

DevAssistAdapter* GetAdapter()
{
	if( !g_pAdapter )
	{
		g_pAdapter = new DllLoadAdapter();
	}
	return g_pAdapter;
}


void ComAdapter::ShowRegistrationMsg()
{
	::MessageBox(0, L"Register the DevAssistIF.dll or\r\n"
					L"Setup installation path as below\r\n"
					L"HKEY_CURRENT_USER\\SOFTWARE\\DevAssist<InstallDir>", 
					L"Quick Launch",MB_OK | MB_ICONINFORMATION );

	// Fail safe
	g_pAdapter = new DllLoadAdapter();
}



//long ToggleImpl_load( LPCTSTR lpFileName, LPTSTR lpOut, int Length_i )
//{
//	InitDll();
//	if( pToggle )
//	{
//		return pToggle( lpFileName, Length_i, lpOut );
//	}
//	return -1;
//}
//long ToggleImpl_COM( LPTSTR filename, LPTSTR lpOut, int Length_i )
//{
//		LONG lReturn = -1;
//		CoInitialize(0);
//
//	HRESULT hr;                    //COM error code;
//	IDevAssistCOM *chat;    //pointer to the interface
//
//	hr = CoCreateInstance(
//		CLSID_DevAssistCOM,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_IDevAssistCOM,
//		(void**) &chat);
//	if(SUCCEEDED(hr))
//	{
//		/*
//		**    STEP 4
//		** call method via Interface ID.
//		*/  
//
//	
//		BSTR Path_o; 
//		hr = chat ->ToggleFile( filename, &Path_o, &lReturn );
//		if( BRIDGE_SUCCESS == lReturn )
//		{
//			int which = -1;
//			lstrcpy( lpOut, Path_o );
//			::SysFreeString(Path_o);
//		}
//		//for( int nInfx = 0; nInfx < 1024; ++nInfx )
//		//{
//		//	BSTR message; 
//		//	BSTR message1(str.AllocSysString()); 
//		//	hr = chat ->GetFilePath( message1, &message, &lReturn );
//		//	::SysFreeString(message1);
//		//	::SysFreeString(message);
//		//}
//		// ::MessageBox(NULL, message, L"Message returned from chat server", MB_OK );
//        // ::SysFreeString(*message);
//
//	//	hr = chat -> Release();
//	}
//
//	CoUninitialize();
//	return lReturn;
//}
//long OpenFileImpl_COM ( LPTSTR filename, LPTSTR lpOut, int Length_i )
//{
//	CoInitialize(0);
//	HRESULT hr;                    //COM error code;
//	IDevAssistCOM *chat;    //pointer to the interface
//	LONG lReturn = -1;
//	hr = CoCreateInstance(
//		CLSID_DevAssistCOM,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_IDevAssistCOM,
//		(void**) &chat);
//	if(SUCCEEDED(hr))
//	{
//		/*
//		**    STEP 4
//		** call method via Interface ID.
//		*/  
//
//		
//		BSTR Path_o; 
//		hr = chat ->GetFilePath( filename, &Path_o, &lReturn );
//		if( BRIDGE_SUCCESS == lReturn )
//		{
//			int which = -1;
//			lstrcpy( lpOut, Path_o );
//			::SysFreeString(Path_o);
//		}		
//
//		//for( int nInfx = 0; nInfx < 1024; ++nInfx )
//		//{
//		//	BSTR message; 
//		//	BSTR message1(str.AllocSysString()); 
//		//	hr = chat ->GetFilePath( message1, &message, &lReturn );
//		//	::SysFreeString(message1);
//		//	::SysFreeString(message);
//		//}
//		// ::MessageBox(NULL, message, L"Message returned from chat server", MB_OK );
//        // ::SysFreeString(*message);
//
//	//	hr = chat -> Release();
//	}
//
//	CoUninitialize();
//	return lReturn;
//}
//long ShowSettingsImpl_COM()
//{
//	CoInitialize(0);
//	//used to accept return value from server.
//    HRESULT hr;                    //COM error code;
//	IDevAssistCOM *chat;    //pointer to the interface
//	LONG lReturn = -1;
//
//	hr = CoCreateInstance(
//		CLSID_DevAssistCOM,
//		NULL,
//		CLSCTX_INPROC_SERVER,
//		IID_IDevAssistCOM,
//		(void**) &chat);
//	if(SUCCEEDED(hr))
//	{
//		/*
//		**    STEP 4
//		** call method via Interface ID.
//		*/  
//
//		
//		hr = chat ->ShowSettingsDlg( &lReturn );
//
//		//for( int nInfx = 0; nInfx < 1024; ++nInfx )
//		//{
//		//	BSTR message; 
//		//	BSTR message1(str.AllocSysString()); 
//		//	hr = chat ->GetFilePath( message1, &message, &lReturn );
//		//	::SysFreeString(message1);
//		//	::SysFreeString(message);
//		//}
//		// ::MessageBox(NULL, message, L"Message returned from chat server", MB_OK );
//        // ::SysFreeString(*message);
//
//	//	hr = chat -> Release();
//	}
//
//	CoUninitialize();
//	return lReturn;
//}

void ToggleFile()
{
	TCHAR filename[MAX_PATH];
	filename[0] = '\0';
	::SendMessage(nppData._nppHandle, NPPM_GETFILENAME, (WPARAM)MAX_PATH,  (LPARAM)filename);


	TCHAR filename_out[MAX_PATH];
	long lReturn = GetAdapter()->ToggleImpl( filename, filename_out, MAX_PATH );
	if( BRIDGE_ESC == lReturn || BRIDGE_NO_TOGGLE  == lReturn )
	{
		// Nothing to do.
	}
	else if( BRIDGE_SUCCESS == lReturn )
	{
		int which = -1;
		::SendMessage(nppData._nppHandle, NPPM_DOOPEN, (WPARAM)&which,(LPARAM)filename_out );		
	}
	else
	{
		::MessageBox(NULL,  L"Failed to find the file", L"Quick launch" , MB_OK | MB_ICONERROR  );
	}
}

void OpenFile()
{
	TCHAR filename[MAX_PATH];
	filename[0] = '\0';

	//::SendMessage(nppData._nppHandle, NPPM_GETFILENAME, (WPARAM)MAX_PATH,  (LPARAM)filename);
	// ::MessageBox(NULL, filename, TEXT("NPPM_GETFILENAME"), MB_OK);
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTWORD, (WPARAM)MAX_PATH,  (LPARAM)filename);
	
	TCHAR fileOut[MAX_PATH];
	LONG lReturn = GetAdapter()->OpenFileImpl(filename, fileOut, MAX_PATH );

	if( BRIDGE_ESC == lReturn )
	{
		// Nothing to do.
	}
	else if( BRIDGE_SUCCESS != lReturn )
	{
		::MessageBox(NULL,  L"Failed to find the file", L"Quick launch" , MB_OK | MB_ICONERROR  );
	}
	else
	{
		// ::MessageBox(NULL, Path_o, L"Message returned from chat server", MB_OK );
		int which = -1;
		::SendMessage(nppData._nppHandle, NPPM_DOOPEN, (WPARAM)&which,(LPARAM)fileOut );
	}
}




void ShowSettings()
{
	LONG lReturn = GetAdapter()->ShowSettingsImpl();
	if( BRIDGE_SUCCESS != lReturn )
	{
		::MessageBox(NULL,  L"Failed to show settings", L"Quick launch" , MB_OK | MB_ICONERROR  );
	}

	//// Open a new document
	//::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

	//// Get the current scintilla
	//int which = -1;
	//::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
	//if (which == -1)
	//    return;
	//HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

	//// Say hello now :
	//// Scintilla control has no Unicode mode, so we use (char *) here
	//::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
}

void OpenContextMenu()
{
	// ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);

	int which = -1;


	TCHAR filename[MAX_PATH];
	//::SendMessage(nppData._nppHandle, NPPM_GETFILENAME, (WPARAM)MAX_PATH,  (LPARAM)filename);
	// ::MessageBox(NULL, filename, TEXT("NPPM_GETFILENAME"), MB_OK);
	//::SendMessage(nppData._nppHandle, NPPM_GETCURRENTWORD, (WPARAM)MAX_PATH,  (LPARAM)filename);
	// ::MessageBox(NULL, filename, TEXT("NPPM_GETCURRENTWORD"), MB_OK);

	//::SendMessage(nppData._nppHandle, NPPM_GETEXTPART, (WPARAM)MAX_PATH,  (LPARAM)filename);
	// ::MessageBox(NULL, filename, TEXT("NPPM_GETEXTPART"), MB_OK);


	// Open a new document
	//	::SendMessage(nppData._nppHandle, NPPM_DOOPEN, (WPARAM)&which,(LPARAM)L"C:\\build.log" );




	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, (WPARAM)MAX_PATH,  (LPARAM)filename);
	//	::MessageBox(NULL, filename, TEXT("NPPM_GETFULLCURRENTPATH"), MB_OK);

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	openShellContextMenuForObject( filename, cursorPos.x,cursorPos.y, nppData._nppHandle );
	// 	OnContextMenu( nppData._nppHandle, filename, cursorPos.x,cursorPos.y);
}

