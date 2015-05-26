#include "stdafx.h"
#include <DirectoryParser.h>


#include <eh.h>              // For _set_se_translator
class CSE {
public:
	// Call this function for each thread.
	static void MapSEtoCE() { _set_se_translator(TranslateSEtoCE); }
	operator DWORD() { return(m_er.ExceptionCode); }
private:
	CSE(PEXCEPTION_POINTERS pep) {
		m_er      = *pep->ExceptionRecord;
		m_context = *pep->ContextRecord;
	}
	static void _cdecl TranslateSEtoCE(UINT dwEC, 
		PEXCEPTION_POINTERS pep) {
			throw CSE(pep);
	}
private:
	EXCEPTION_RECORD m_er;      // CPU independent exception information
	CONTEXT          m_context; // CPU dependent exception information
};


CDirectoryParser::CDirectoryParser( )
{}

void CDirectoryParser::AddInterestedExtensions( LPCTSTR lpExtWithoutDot_i )
{
	mlstIntrExtn.AddTail( lpExtWithoutDot_i );
}

bool CDirectoryParser::ParseDirectory( LPCTSTR lpDirectory_i, 
									  CStringArray& lstDirectories_o, 
									  CStringArray& lstFiles_o )
{
	// EHa is enabled
	CSE::MapSEtoCE(); 
	try
	{

		return ParseDirectoryImpl(lpDirectory_i, lstDirectories_o, lstFiles_o );
	}
	// __except(EXCEPTION_EXECUTE_HANDLER)
	catch(CSE se)
	{
		// AfxMessageBox( _T("Crash" ));
		return false;
	}
}

bool CDirectoryParser::ParseDirectoryImpl( LPCTSTR lpDirectory_i, 
										  CStringArray& lstDirectories_o, 
										  CStringArray& lstFiles_o )
{		
	int nIntrAdded = mlstIntrExtn.GetCount();
	{
		CFileFind finder;

		// build a string with wildcards
		// CString strWildcard(pstr);
		CString csPath(lpDirectory_i);
		csPath.TrimRight( _T( "\\" ));
		csPath += _T("\\*.*");

		// start working for files
		BOOL bWorking = finder.FindFile(csPath);
		if( !bWorking )
		{
			return false;
		}

		bool bFirst = true;
		UINT uPath = 0;
		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files; 

			if (finder.IsDots() || finder.IsSystem() || finder.IsHidden() || finder.IsTemporary() )
				continue;

			// if it's a directory, recursively search it 			
			if (finder.IsDirectory())
			{
				lstDirectories_o.Add(finder.GetFileName());
			}
			else
			{	
				CString csFileName = finder.GetFileName();
				POSITION posEx = 0;

				if(nIntrAdded )
				{
					int nExt = csFileName.ReverseFind('.' );		
					if( -1 != nExt )
					{
						CString csExtn = csFileName.Mid( nExt+1 );
						posEx = mlstIntrExtn.Find( csExtn );
						if( posEx )
						{
							lstFiles_o.Add(csFileName);
						}
					}
				}
				else
				{
					lstFiles_o.Add(csFileName);
				}			
			}
		}
		finder.Close();
	}
	// catch( ... )
	// __except( EXCEPTION_EXECUTE_HANDLER )
	//{
	//	AfxMessageBox( _T("Crash" ));
	//	return false;
	// }
	return true;
}

