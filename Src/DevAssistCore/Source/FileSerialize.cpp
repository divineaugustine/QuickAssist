#include "stdafx.h"
#include "FileSerialize.h"
#include <FileRepository.h>

	extern LPCTSTR DEVASSIST_REPOSITORY;
	std::wstring GetRepoPath(  ConfigI* pConf, const RepositoryName& pName_i )
	{
		std::wstring wsFolder(pConf->GetInstalledPath());
		wsFolder += DEVASSIST_REPOSITORY;
		wsFolder += pName_i.GetName();
		wsFolder += L"\\FileList.rpo";
		return wsFolder;
	}

	bool FileSerialize::Serialize(  FileRepository* pFile )
	{
		std::wstring wsPath = GetRepoPath(  mpConf, mpConf->GetCurrentRepository() );
		return pFile->WriteToFile( wsPath );
	}

	bool FileSerialize::Deserialize( FileRepository* pFile )
	{
		std::wstring wsPath = GetRepoPath(  mpConf, mpConf->GetCurrentRepository() );
		return pFile->ReadFromFile( wsPath );
	}

	FileSerialize::FileSerialize(  ConfigI* pConf) : mpConf(pConf)
	{}
	FileSerialize::~FileSerialize(){};