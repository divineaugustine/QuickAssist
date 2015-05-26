
#include <stdafx.h>
#include <Core\ConfigI.h>
#include <Core\DataStructs.h>

	const std::wstring& RepositoryName::GetName() const
	{
		return mcsName;
	}

	void RepositoryName::SetName(const std::wstring& csName)
	{
		mcsName = csName;
	}

	RepositoryName::RepositoryName( ){}
	RepositoryName::~RepositoryName(){}

	bool RepositoryName::IsValid()
	{
		return mcsName.size() > 0 ? true : false;
	}

	bool RepositoryName::operator == ( const RepositoryName& src)
	{
		return mcsName == src.mcsName;
	}

	bool RepositoryName::operator != ( const RepositoryName& src)
	{
		return mcsName != src.mcsName;
	}

	bool RepositoryName::operator<(const RepositoryName& name) const
	{
		return mcsName < name.mcsName;
	}

	const std::wstring& BaseDirectory::GetPath() const
	{
		return mcsPath;
	}

	void BaseDirectory::SetPath(const std::wstring& csName)
	{
		mcsPath = csName;
	}

	BaseDirectory::BaseDirectory( ){}
	BaseDirectory::~BaseDirectory(){}

	bool BaseDirectory::IsValid()
	{
		return mcsPath.size() > 0 ? true : false;
	}

	bool BaseDirectory::operator == ( const BaseDirectory& src)
	{
		return mcsPath == src.mcsPath;
	}

	bool BaseDirectory::operator != ( const BaseDirectory& src)
	{
		return mcsPath != src.mcsPath;
	}

	int BaseDirectory::ResolveVirtualPath( DirectoryInfo* pDirInfo_i, TCHAR* pBuff_o )
	{
		std::wstring csVirtual_o;
		csVirtual_o = pDirInfo_i->GetPath();
		std::wstring ws(L"?");
		csVirtual_o.replace(csVirtual_o.find(ws),ws.length(),GetPath());
		if( pBuff_o )
		{
			lstrcpy( pBuff_o, csVirtual_o.c_str());
		}
		return csVirtual_o.size();
	}



	const std::vector<std::wstring>& RepositorySettings::GetConfiguredFolders() const
	{
		return mvecConfigFOlders;
	}

	void RepositorySettings::SetConfiguredFolders( std::vector<std::wstring>& vecConfiguredFolderList )
	{
		// mvecConfigFOlders.clear();
		mvecConfigFOlders = vecConfiguredFolderList;		
	}

	const std::vector<std::wstring>& RepositorySettings::GetSupportedExtensions()  const
	{
		return mvecExtensions;
	}

	void RepositorySettings::SetSupportedExtensions(std::vector<std::wstring>& vecSupportedExtns )
	{
		mvecExtensions = vecSupportedExtns;
	}

	const std::vector<std::wstring>& RepositorySettings::GetIgnoreDirectories()  const
	{
		return mvecIgnoreDirs;
	}

	void RepositorySettings::SetIgnoreDirectories(std::vector<std::wstring>& vecIgnoreDirs )
	{
		mvecIgnoreDirs = vecIgnoreDirs;
	}