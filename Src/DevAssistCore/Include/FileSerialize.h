#pragma once
#include <Core\Repository.h>
#include <Core\ConfigI.h>
class FileSerialize : public SerialStrategyBase
{
public:
	
	bool Serialize(  FileRepository* pFile );

	bool Deserialize( FileRepository* pFile );

	FileSerialize(  ConfigI* pConf);
	~FileSerialize();
private:
	FileSerialize(const FileSerialize& src);
	FileSerialize& operator=(const FileSerialize& src);

private:
	 ConfigI* mpConf;

};