#pragma once
#include <Core\Datastructs.h>
#include <map>
// #include <FileInfoImpl.h>
class FileInfoImpl;

class FileIteratorImpl : public FileIterator
{
public:

	FileIteratorImpl( std::map<std::wstring,FileInfoImpl*>* pFileRep);
	FileInfo* Next( );
	bool HasNext();;
	~FileIteratorImpl();

private:

	std::map<std::wstring,FileInfoImpl*>* mpFileRep;
	std::map<std::wstring,FileInfoImpl*>::iterator mBegin;
};
