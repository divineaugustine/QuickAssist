#pragma once 
#include "Defs.h"
#include "DataStructs.h"

class ConfigI;
class SerialStrategyBase;
class RepositoryBase;

class CORE_API DevAssistFactory
{
public:
	
	static DevAssistFactory& Instance();

	virtual ConfigI* GetConfig() = 0;
	virtual SerialStrategyBase* GetSerializationStrategy() = 0;
	virtual RepositoryBase* GetRepository() = 0;
	virtual ISearch<FileInfo>* CreateThreadSearch( std::shared_ptr<Iterator<FileInfo>> pIterator, IResultContainer<FileInfo>* pContainer ) = 0;



protected:
	DevAssistFactory();
	virtual ~DevAssistFactory();
private:
	DevAssistFactory(const DevAssistFactory& src);
	DevAssistFactory& operator=(const DevAssistFactory& src);
};