

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Fri May 22 12:54:09 2015
 */
/* Compiler settings for DevAssistIF.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DevAssistIF_i_h__
#define __DevAssistIF_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDevAssistCOM_FWD_DEFINED__
#define __IDevAssistCOM_FWD_DEFINED__
typedef interface IDevAssistCOM IDevAssistCOM;

#endif 	/* __IDevAssistCOM_FWD_DEFINED__ */


#ifndef __DevAssistCOM_FWD_DEFINED__
#define __DevAssistCOM_FWD_DEFINED__

#ifdef __cplusplus
typedef class DevAssistCOM DevAssistCOM;
#else
typedef struct DevAssistCOM DevAssistCOM;
#endif /* __cplusplus */

#endif 	/* __DevAssistCOM_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IDevAssistCOM_INTERFACE_DEFINED__
#define __IDevAssistCOM_INTERFACE_DEFINED__

/* interface IDevAssistCOM */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IDevAssistCOM;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A4FA98B5-1571-4D6C-911E-DC60FCAF1A2C")
    IDevAssistCOM : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ShowSettingsDlg( 
            /* [out] */ LONG *lStatus_o) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetFilePath( 
            /* [in] */ BSTR pFileName,
            /* [out] */ BSTR *lFilePath_o,
            /* [out] */ LONG *lStatus_o) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ToggleFile( 
            BSTR strCurrentFile,
            /* [out] */ BSTR *Path_o,
            /* [out] */ LONG *lStatus_o) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IDevAssistCOMVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDevAssistCOM * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDevAssistCOM * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDevAssistCOM * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDevAssistCOM * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDevAssistCOM * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDevAssistCOM * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDevAssistCOM * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ShowSettingsDlg )( 
            IDevAssistCOM * This,
            /* [out] */ LONG *lStatus_o);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetFilePath )( 
            IDevAssistCOM * This,
            /* [in] */ BSTR pFileName,
            /* [out] */ BSTR *lFilePath_o,
            /* [out] */ LONG *lStatus_o);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *ToggleFile )( 
            IDevAssistCOM * This,
            BSTR strCurrentFile,
            /* [out] */ BSTR *Path_o,
            /* [out] */ LONG *lStatus_o);
        
        END_INTERFACE
    } IDevAssistCOMVtbl;

    interface IDevAssistCOM
    {
        CONST_VTBL struct IDevAssistCOMVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDevAssistCOM_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDevAssistCOM_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDevAssistCOM_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDevAssistCOM_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDevAssistCOM_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDevAssistCOM_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDevAssistCOM_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDevAssistCOM_ShowSettingsDlg(This,lStatus_o)	\
    ( (This)->lpVtbl -> ShowSettingsDlg(This,lStatus_o) ) 

#define IDevAssistCOM_GetFilePath(This,pFileName,lFilePath_o,lStatus_o)	\
    ( (This)->lpVtbl -> GetFilePath(This,pFileName,lFilePath_o,lStatus_o) ) 

#define IDevAssistCOM_ToggleFile(This,strCurrentFile,Path_o,lStatus_o)	\
    ( (This)->lpVtbl -> ToggleFile(This,strCurrentFile,Path_o,lStatus_o) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDevAssistCOM_INTERFACE_DEFINED__ */



#ifndef __DevAssistIFLib_LIBRARY_DEFINED__
#define __DevAssistIFLib_LIBRARY_DEFINED__

/* library DevAssistIFLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_DevAssistIFLib;

EXTERN_C const CLSID CLSID_DevAssistCOM;

#ifdef __cplusplus

class DECLSPEC_UUID("DABCA1B1-3C03-48B3-B22C-C2ED3A5876CA")
DevAssistCOM;
#endif
#endif /* __DevAssistIFLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


