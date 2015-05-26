#include "stdafx.h"
#include <FileFactory.h>

#include <Core/DataStructs.h>
#include <Core/Repository.h>
#include <Core/ConfigI.h>
#include "ConfigImpl.h"
#include "FileSerialize.h"
#include "FileRepository.h"
#include "ThreadSearch.h"

	ConfigI* FileFactory::GetConfig()
	{
		if( 0 == mpConf )
		{
			mpConf = new ConfigImpl();
		}
		return mpConf;
	}

	SerialStrategyBase* FileFactory::GetSerializationStrategy()
	{
		if( 0 == mpSerialStrategy )
		{
			mpSerialStrategy = new FileSerialize(GetConfig());
		}
		return mpSerialStrategy;
	}

	RepositoryBase* FileFactory::GetRepository()
	{
		if( 0 == mpRepository )
		{
			mpRepository = new FileRepository();
		}
		return mpRepository;
	}

	ISearch<FileInfo>* FileFactory::CreateThreadSearch( std::shared_ptr<Iterator<FileInfo>> pIterator,IResultContainer<FileInfo>* pContainer ) 
	{
		return new ThreadSearch(pIterator, pContainer );
	}

	FileFactory::FileFactory()	 : mpConf(0)
	 ,mpSerialStrategy(0)
	 ,mpRepository(0)
	{
		SerialStrategyBase* pStrat = GetSerializationStrategy();
		RepositoryBase* pRep = GetRepository();
		pRep->Deserialize( pStrat );
	}

	FileFactory::~FileFactory()
	{}