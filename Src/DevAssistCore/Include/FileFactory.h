#pragma once 
#include <Core\DevAssistFactory.h>

class  FileFactory : public DevAssistFactory
{
public:

	virtual ConfigI* GetConfig();
	virtual SerialStrategyBase* GetSerializationStrategy();
	virtual RepositoryBase* GetRepository();
	virtual ISearch<FileInfo>* CreateThreadSearch( std::shared_ptr<Iterator<FileInfo>> pIterator,IResultContainer<FileInfo>* pContainer ) ;

	FileFactory();
	virtual ~FileFactory();
private:

	FileFactory(const FileFactory& src);
	FileFactory& operator=(const FileFactory& src);

private:

	 ConfigI* mpConf;
	 SerialStrategyBase* mpSerialStrategy;
	 RepositoryBase* mpRepository;
};