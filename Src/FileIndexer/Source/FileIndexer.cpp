// FileIndexer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DirectoryParserMgr.h"

#include <Core\DevAssistFactory.h>
#include <Core\ConfigI.h>
#include <Bridge\BridgeIF.h>
#include <Core\Repository.h>

void Log( LPCTSTR lpInfo )
{
	_tprintf( L"%s\n",  lpInfo );
}



int _tmain(int argc, _TCHAR* argv[])
{

	DevAssistFactory& obj = DevAssistFactory::Instance();
	ConfigI* pConfig = obj.GetConfig();
	BaseDirectory base = pConfig->GetBaseDirectory();
	if( base.IsValid() == 0 )
	{
		Log( L"Base directory is not specified \n" );
		return 0;
	}
	CDirectoryParserMgr::Instance().SetReplacePath( base.GetPath().c_str());
	CDirectoryParserMgr::Instance().SetLogger( Log );
	CDirectoryParserMgr::Instance().StartParsing( );
	return 0;
}

