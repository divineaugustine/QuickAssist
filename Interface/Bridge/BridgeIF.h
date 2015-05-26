#pragma once
#ifdef DEVASSISTBRIDGEDLL_EXPORTS
#define BRIDGE_API __declspec(dllexport) _cdecl
#else
#define BRIDGE_API __declspec(dllimport)  _cdecl
#endif


const long BRIDGE_SUCCESS = 0x0;
const long BRIDGE_UNKNOWN_ERROR = 0x1;
const long BRIDGE_ESC = 0x2;
const long BRIDGE_NO_TOGGLE = 0x2;

extern "C" long BRIDGE_API  ToggleSoruce( LPCWSTR fileName, int nOutBufferSize_i, LPWSTR lpFile_o );
extern "C" long BRIDGE_API   OpenSettings();
extern "C" long BRIDGE_API  SearchFile( LPCWSTR fileName, int nOutBufferSize_i, LPWSTR lpFile_o );
