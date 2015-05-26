#include "stdafx.h"
#include "FileRepository.h"
#include <FileInfoImpl.h>
#include <DirectoryInfoImpl.h>
#include <FileIteratorImpl.h>

const int CurrentVersion = 1;
char szHeader[] = "JC";


FileRepository::FileRepository() : mnFolderCounter(0)
{
}

FileRepository::~FileRepository()
{
	ClearAll();
}

bool FileRepository::Serialize( SerialStrategyBase*  pSerialStrategy )
{
	return pSerialStrategy->Serialize( this );
}
bool FileRepository::Deserialize( SerialStrategyBase*  pSerialStrategy )
{
	return pSerialStrategy->Deserialize( this );
}

void FileRepository::ResetContents()
{
	ClearAll();
}

FileInfo* FileRepository::GetFile(  const std::wstring& FileName_i )
{
	// CString csFile( FileName_i.c_str());
	std::map<std::wstring,FileInfoImpl*>::iterator it = mFileRep.find( FileName_i );
	if(  mFileRep.end() == it )
	{
		return 0;
	}
	else
	{
		FileInfoImpl* pFileInfo = it->second;
		return pFileInfo;
	}
}

std::shared_ptr<FileIterator> FileRepository::CreateFileIterator()
{
	std::shared_ptr<FileIteratorImpl> obj =  std::make_shared<FileIteratorImpl>(&mFileRep);
	return obj;
}

DirectoryInfo* FileRepository::AddFolder( const std::wstring& strFolder_i )
{
	// CSingleLock objLock( &mObjCritic, TRUE );

	// Not considering a duplicate folder for performance reason
	// ++mnFolderCounter;
	// mFolderRep[strFolder_i] = mnFolderCounter;
	if( UINT_MAX == mnFolderCounter )
	{
		return 0;
	}

	DirectoryInfoImpl* pDir = new DirectoryInfoImpl(strFolder_i, mnFolderCounter);
	mFolderRep.push_back(pDir);
	mnFolderCounter++;
	return pDir;
	// objLock.Unlock();
}


void FileRepository::AddFile( const std::wstring& strFile_i, DirectoryInfo* nFolderID_i )
{
	// Check if same file has an entry already
	std::map<std::wstring,FileInfoImpl*>::iterator it = mFileRep.find( strFile_i );
	if(  mFileRep.end() == it )
	{
		FileInfoImpl* pFileInfo = new FileInfoImpl(strFile_i);
		pFileInfo->AddDirectory( nFolderID_i);
		mFileRep[strFile_i] = pFileInfo;
	}
	else
	{
		FileInfoImpl* pFileInfo = it->second;
		if( pFileInfo )
		{
			pFileInfo->AddDirectory( nFolderID_i );
		}
	}
	return;
}


void FileRepository::ClearAll()
{
	try
	{
		for (std::map<std::wstring,FileInfoImpl*>::iterator it = mFileRep.begin() ; it != mFileRep.end(); ++it)
		{
			delete it->second;
		}

		for (std::vector<DirectoryInfoImpl*>::iterator it2 = mFolderRep.begin() ; it2 != mFolderRep.end(); ++it2)
		{
			delete (*it2);
		}
	}
	catch( ... )
	{
	}
	mFileRep.clear();
	mFolderRep.clear();
	mnFolderCounter = 0;
}

struct strFH
{
	char fhh[4];
	int nVersionNum;
	unsigned int nTotalFileCount;
	unsigned int nFolderCount;
};

bool CopyToVector(std::vector<BYTE>& vec, int nNewSize, LPBYTE pBuff )
{
	int nsize = vec.size();
	int ncap = vec.capacity();
	if(nsize +nNewSize >= ncap)
	{
		// _tprintf(L"size=%d, capacity = %d\n", nsize,ncap);
		vec.reserve(ncap*2);		
	}	

	vec.insert( vec.end() , pBuff , pBuff + nNewSize ) ; 
	return true;
}

bool FileRepository::WriteToFile( const std::wstring& strDestFile_i )
{
	try
	{
		std::vector<BYTE> vecFileRep;
		// Reserving 4MB
		vecFileRep.reserve( 1024*4 );

		/*CFile objFile;
		if( FALSE == objFile.Open(strDestFile_i.c_str(), CFile::modeCreate | CFile::modeWrite ))
		{
		return false;
		}*/

		strFH st;
		strcpy_s( st.fhh, szHeader );
		st.nVersionNum = CurrentVersion;
		st.nTotalFileCount = mFileRep.size();
		st.nFolderCount= mFolderRep.size();

		// objFile.Write( &st, sizeof(st));
		CopyToVector(vecFileRep, sizeof(st), (LPBYTE)&st );

		// Write Folder contents
		// objFile.Write( &st.nFolderCount, sizeof(st.nFolderCount));
		CopyToVector(vecFileRep, sizeof(st.nFolderCount), (LPBYTE)&st.nFolderCount );

		for(std::vector<DirectoryInfoImpl*>::iterator itFol = mFolderRep.begin() ; itFol != mFolderRep.end(); ++itFol)
		{
			std::wstring folderName = (*itFol)->GetPath();
			const WCHAR* lpFileName = folderName.c_str();
			USHORT nSize = (folderName.size() +1)*sizeof(WCHAR);
			//objFile.Write( lpFileName, nSize );
			CopyToVector(vecFileRep, nSize, (LPBYTE)lpFileName );
		}

		//objFile.Write( &st.nTotalFileCount, sizeof(st.nTotalFileCount));
		CopyToVector(vecFileRep, sizeof(st.nTotalFileCount), (LPBYTE)&st.nTotalFileCount );

		for(std::map<std::wstring,FileInfoImpl*>::iterator it = mFileRep.begin() ; it != mFileRep.end(); ++it)
		{
			std::wstring fileName = it->first;
			FileInfoImpl* pFolderList = it->second;
			if( pFolderList )
			{
				const WCHAR* lpFileName = fileName.c_str();
				int nSize = (fileName.size() +1)*sizeof(WCHAR);
				// Folder count includes the count field
				unsigned int nFolderCount = pFolderList->GetDirectoryCount();

				int nFileSize = nSize + (nFolderCount+1)*sizeof(unsigned int);
				std::auto_ptr<BYTE> pBuff( new BYTE[nFileSize]);
				memcpy( pBuff.get(), lpFileName, nSize );
				memcpy( pBuff.get()+nSize, &nFolderCount, sizeof(nFolderCount));

				std::shared_ptr<DirectoryIterator> pIter = pFolderList->CreateIterator();
				int nSizeCount = nSize +sizeof(nFolderCount);
				while( pIter->HasNext())
				{
					DirectoryInfoImpl* pDirec = (DirectoryInfoImpl*)pIter->Next();
					unsigned int nFOlderIndx = pDirec->GetIndex();
					memcpy( pBuff.get()+nSizeCount, &nFOlderIndx, sizeof(nFOlderIndx));
					nSizeCount+= sizeof(nFOlderIndx);;
				}
				// objFile.Write( pBuff.get(), nFileSize );
				CopyToVector(vecFileRep, nFileSize, (LPBYTE)pBuff.get() );
			}
		}


		HANDLE hMutex = CreateMutex( NULL, 
							  TRUE, 
							  L"FILEREPO_FILE_ROLL");
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			WaitForSingleObject( hMutex, INFINITE );
		}

		try
		{
			CFile objFile2;
			if( FALSE == objFile2.Open(strDestFile_i.c_str(), CFile::modeCreate | CFile::modeWrite ))
			{
				ReleaseMutex( hMutex );
				CloseHandle( hMutex );
				return false;
			}
			objFile2.Write( &vecFileRep[0], vecFileRep.size());
		}
		catch( ... )
		{
		}

		ReleaseMutex( hMutex );
		CloseHandle( hMutex );
	}
	catch( ... )
	{
		return false;
	}
	return true;
}


bool FileRepository::ReadFromFile( const std::wstring& strDestFile_i )
{
	try
	{
		ClearAll();

		HANDLE hMutex = CreateMutex( NULL, 
			TRUE, 
			L"FILEREPO_FILE_ROLL");
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			WaitForSingleObject( hMutex, INFINITE );
		}

		CFile obj;
		if( !obj.Open( strDestFile_i.c_str(), CFile::modeRead ))
		{
			ReleaseMutex( hMutex );
			CloseHandle( hMutex );
			return false;
		}
		std::auto_ptr<BYTE> pFileContent( new BYTE[obj.GetLength()+2]);
		// BYTE* pFileContent = new BYTE[obj.GetLength()+2];
		obj.Read( pFileContent.get(), obj.GetLength());
		obj.Close();
		ReleaseMutex( hMutex );
		CloseHandle( hMutex );


		strFH* pstHeader = (strFH*)(pFileContent.get());
		if( 0 != strcmp( pstHeader->fhh, szHeader ) ||
			CurrentVersion != pstHeader->nVersionNum )
		{
			// delete[] pFileContent;
			// File is either old version or corrupted.
			return false;
		}

		BYTE* psz = (BYTE*)((pFileContent.get())+sizeof(*pstHeader));

		unsigned int nTotalFolders = *(unsigned int*)(psz);
		if( nTotalFolders != pstHeader->nFolderCount )
		{
			// delete[] pFileContent;
			return false;
		}
		psz +=sizeof(nTotalFolders);
		for(  unsigned int nIndx = 0 ; nIndx < nTotalFolders; ++nIndx )
		{
			std::wstring cs((WCHAR*)psz);
			int nLen = lstrlen( (WCHAR*)psz )*sizeof(WCHAR);
			nLen+=2;
			psz += nLen;
			AddFolder( cs );
		}
		unsigned int nTotalFiles = *(unsigned int*)(psz);
		psz +=sizeof(nTotalFiles);
		if( nTotalFiles != pstHeader->nTotalFileCount )
		{
			ClearAll();
			// delete[] pFileContent;
			return false;
		}

		for( unsigned int nIndx = 0 ; nIndx < pstHeader->nTotalFileCount; ++nIndx )
		{
			//CString cs(psz);
			std::wstring wsFileName((WCHAR*)psz);
			int nLen = lstrlen( (WCHAR*)psz ) *sizeof(WCHAR);

			// terminating null 
			nLen+=2;
			psz += (nLen);

			unsigned int nFolderCount = *(unsigned int*)psz;

			psz += sizeof(nFolderCount);
			unsigned int nSmVal2 = 0;

			// std::vector<unsigned int>* pFolderList = new std::vector<unsigned int>;
			// pFolderList->push_back( nFolderCount );

			FileInfoImpl* pFileIfoIm = new FileInfoImpl(wsFileName);
			mFileRep[wsFileName] = pFileIfoIm;
			for(  unsigned int Sub = 0; Sub < nFolderCount; ++ Sub )
			{
				nSmVal2 = *( unsigned int*)(psz);
				psz += sizeof(nSmVal2);
				// pFolderList->push_back( nSmVal2 );
				if(nSmVal2 >= pstHeader->nFolderCount )
				{
					ClearAll();
					// delete[] pFileContent;
					return false;
				}
				DirectoryInfoImpl* pDirInfo = mFolderRep[nSmVal2];
				pFileIfoIm->AddDirectory(pDirInfo);
			}
		}

	}
	catch( ... )
	{
		return false;
	}
	// delete[] pFileContent;
	return true;
}