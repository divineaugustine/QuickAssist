#pragma once
#include "DirectoryParser.h"
// class RepositorySettings;

typedef void (*DIR_PARSE_ERROR_FN)( LPCTSTR ) ;
class CDirectoryParserMgr
{
public:
	static CDirectoryParserMgr& Instance();
	bool StartParsing();
	void SetReplacePath( LPCTSTR lpReplaceStatic_i )
	{
		mcsReplaceStatic = lpReplaceStatic_i;
	}

	void SetLogger( DIR_PARSE_ERROR_FN lpLogger )
	{
		mfnCallBack = lpLogger;
	}
	

private:
	CDirectoryParserMgr()
	{}
	~CDirectoryParserMgr(){}

	bool ParseFolder( LPCTSTR lpFolder_i , LPCTSTR lpVirtualFolder_i );
	bool IsIgnore( LPCTSTR lpFoledr  );

	CDirectoryParser mobjParse;
	CString mcsReplaceStatic;
	DIR_PARSE_ERROR_FN mfnCallBack;
	// RepositorySettings* mpSettings;


	CMap<CString,LPCTSTR,int,int> mIgnoreFOlders;
};