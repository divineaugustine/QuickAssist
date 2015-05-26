#include <stdafx.h>
#include <SettingsReader.h>
#include <Core\ConfigI.h>

bool Tokenize( CString csInput, LPCTSTR lpToken, std::vector<std::wstring>& strItems_o, bool MakeUpper = false )
{
	if( -1 != csInput.Find(L"!"))
	{
		// Comment
		return false;
	}
	csInput.TrimLeft(lpToken);
	csInput.TrimRight(lpToken);

	if( csInput.IsEmpty())
	{
		return false;
	}

	int nPos = csInput.Find( lpToken );
	int nStart = 0;
	while( -1 != nPos )
	{
		CString csTemp = csInput.Mid( nStart, nPos-nStart );
		if( !csTemp.IsEmpty())
		{
			csTemp.TrimLeft( _T( " " ));
			csTemp.TrimRight( _T( " " ));
			if( MakeUpper )
			{
				csTemp.MakeUpper();
			}
			std::wstring ws(csTemp);
			strItems_o.push_back( ws );
		}
		nStart = nPos +1;
		nPos = csInput.Find( lpToken, nPos+1 );
	}

	CString csTemp = csInput.Mid( nStart, csInput.GetLength());
	if( !csTemp.IsEmpty())
	{
		csTemp.TrimLeft( _T( " " ));
		csTemp.TrimRight( _T( " " ));
		if( MakeUpper )
		{
			csTemp.MakeUpper();
		}
		std::wstring ws(csTemp);
		strItems_o.push_back( ws );
	}
	return true;
}

void Combine( LPCTSTR lpToken, const std::vector<std::wstring>& strItems_i, CString& csOut )
{
	for ( std::vector<std::wstring>::const_iterator  it = strItems_i.begin() ; it != strItems_i.end(); ++it )
	{
		csOut += (*it).c_str();
		csOut += lpToken;
	}
	csOut.TrimRight( lpToken );
}

enum eSettingsType
{
	invalid = 0,
	Ext = 1,
	ignoreFolder = 2,
	Path = 3
};

bool CSettingsReader::ReadSettings( const std::wstring& csSettingsFileName, 
								    RepositorySettings& objSettings_o )
{
	CStdioFile objConfigReader;
	if( !objConfigReader.Open( csSettingsFileName.c_str(), CStdioFile::modeRead ))
	{
		return false;
	}
	CString csLine;
	eSettingsType nMode = invalid; // 1= Ext, 2 = ignoreFolder, 3 = Path

	std::vector<std::wstring> vecFolders;


	while( objConfigReader.ReadString( csLine ))
	{
		if( _T( "[Extensions]" ) == csLine )
		{
			nMode = Ext;
			continue;
		}
		else if( _T( "[IgnoreFolders]" ) == csLine )
		{
			nMode = ignoreFolder;
			continue;
		}
		else if( _T( "[Paths]" ) == csLine )
		{
			nMode = Path;
			continue;
		}
		else
		{
			if( Ext== nMode)
			{
				std::vector<std::wstring> strItems;
				if( Tokenize( csLine, _T( "," ), strItems ))
				{
					objSettings_o.SetSupportedExtensions( strItems );
				}
				continue;
			}
			else if( ignoreFolder== nMode)
			{
				std::vector<std::wstring> strItems;
				if( Tokenize( csLine, _T( "," ), strItems, true ) )
				{
					objSettings_o.SetIgnoreDirectories( strItems );
				}

				continue;
			}
			else if( Path == nMode)
			{
				std::vector<std::wstring> strItems;
				if( Tokenize( csLine, _T( "," ), strItems ))
				{
					 for (std::vector<std::wstring>::iterator it = strItems.begin() ; it != strItems.end(); ++it)
					 {
						 vecFolders.push_back( *it );
					 }
					
				}
				continue;
			}
			else 
			{
				continue;
			}
		}
	}

	objSettings_o.SetConfiguredFolders( vecFolders );
	return true;
}

bool CSettingsReader::WriteSettings( const RepositorySettings& objSettings_i, 
									const std::wstring& csSettingsFileName )
{
	CStdioFile objConfigReader;
	if( !objConfigReader.Open( csSettingsFileName.c_str(), CStdioFile::modeWrite | CStdioFile::modeCreate ))
	{
		return false;
	}

	CString csExtensions;
	Combine( L",", objSettings_i.GetSupportedExtensions() , csExtensions );

	CString csIgnore;
	Combine( L",", objSettings_i.GetIgnoreDirectories(), csIgnore );

	CString csConfigFOlders;
	Combine( L",", objSettings_i.GetConfiguredFolders(), csConfigFOlders );

	CString csContent;
	csContent.Format(L"[Extensions]\n%s\n[IgnoreFolders]\n%s\n[Paths]\n%s", csExtensions, csIgnore, csConfigFOlders ) ;

	objConfigReader.WriteString(csContent);
	return true;
}


