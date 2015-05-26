// DevAssistCOM.h : Declaration of the CDevAssistCOM

#pragma once
#include "resource.h"       // main symbols



#include "DevAssistIF_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CDevAssistCOM

class ATL_NO_VTABLE CDevAssistCOM :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDevAssistCOM, &CLSID_DevAssistCOM>,
	public IDispatchImpl<IDevAssistCOM, &IID_IDevAssistCOM, &LIBID_DevAssistIFLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CDevAssistCOM()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEVASSISTCOM)


BEGIN_COM_MAP(CDevAssistCOM)
	COM_INTERFACE_ENTRY(IDevAssistCOM)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



	STDMETHOD(ShowSettingsDlg)(LONG* lStatus_o);
	STDMETHOD(GetFilePath)(BSTR pFileName, BSTR* lFilePath_o, LONG* lStatus_o);
	STDMETHOD(ToggleFile)(BSTR strCurrentFile, BSTR* Path_o, LONG* lStatus_o);
};

OBJECT_ENTRY_AUTO(__uuidof(DevAssistCOM), CDevAssistCOM)
