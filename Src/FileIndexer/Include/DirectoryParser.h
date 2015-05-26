#pragma once

#include <afxtempl.h>
class CDirectoryParser
{
public:

	CDirectoryParser();

	void AddInterestedExtensions( LPCTSTR lpExtWithoutDot_i );

	bool ParseDirectory( LPCTSTR lpDirectory_i,
			CStringArray& lstDirectories_o, CStringArray& lstFiles_o );
private:

	CDirectoryParser( const CDirectoryParser& obj );
	CDirectoryParser& operator=( const CDirectoryParser& obj );

	bool ParseDirectoryImpl( LPCTSTR lpDirectory_i, 
										   CStringArray& lstDirectories_o, 
										   CStringArray& lstFiles_o );

private:
	CStringList mlstIntrExtn;
};
