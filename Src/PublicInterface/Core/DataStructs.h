#pragma once
#include <string>
#include <memory>
#include "Defs.h"

  

template <class A_Type> 
class CORE_API Iterator
{
public:
	virtual A_Type* Next() =0;
	virtual bool HasNext() = 0;

protected:
	Iterator(){}
	virtual ~Iterator()= 0 {};

private:
	Iterator(const Iterator& src);
	Iterator& operator=(const Iterator& src);
};

class CORE_API DirectoryInfo
{
public:
	virtual const std::wstring& GetPath() = 0;

protected:
	DirectoryInfo(){}
	virtual ~DirectoryInfo(){}

private:
	DirectoryInfo(const DirectoryInfo& src);
	DirectoryInfo& operator=(const DirectoryInfo& src);
};

 typedef Iterator<DirectoryInfo> DirectoryIterator;
/*class DirectoryIterator
{
public:
	virtual ~DirectoryIterator(){}
	virtual DirectoryInfo* Next() =0;
	virtual bool HasNext() = 0;
};*/


class CORE_API FileInfo
{
public:
	virtual const std::wstring& GetFileName() const = 0 ;

	virtual int GetDirectoryCount() = 0;
	virtual std::shared_ptr<DirectoryIterator> CreateIterator() = 0;

protected:

	FileInfo(){}
	virtual ~FileInfo(){}

private:
	FileInfo(const FileInfo& src);
	FileInfo& operator=(const FileInfo& src);
};

/*
class FileIterator
{
public:
	virtual ~FileIterator(){}
	virtual FileInfo* Next() =0;
	virtual bool HasNext() = 0;
};
*/

typedef Iterator<FileInfo> FileIterator;

template <class ContentType>
class  ISearchCrieteria
{
public:
	virtual bool Validate( const ContentType* pContent_i ) = 0;

protected:
	ISearchCrieteria(){}
	virtual ~ISearchCrieteria(){}
};


template <class ContentType>
class  IResultContainer
{
public:
	virtual bool AddResult(const ContentType* pContent_i) = 0;
	virtual void NotifySearchEnd() = 0;
protected:
	IResultContainer(){}
	virtual ~IResultContainer(){}
protected:

};


template <class ContentType>
class CORE_API ISearch
{
protected:

	ISearch( std::shared_ptr<Iterator<FileInfo>> pIterator, IResultContainer<ContentType>* pContainer ) :
		mpContainer(pContainer)
		,mpIterator(pIterator)
		,mpCrieteria(0)
	{
	}

public:

	virtual bool StartSearch(std::shared_ptr<Iterator<FileInfo>> pIterator, ISearchCrieteria<ContentType>* pCrieteria) = 0;
	virtual bool Stop() = 0;

protected:

	ISearchCrieteria<ContentType>* mpCrieteria;
	IResultContainer<ContentType>* mpContainer;
	std::shared_ptr<Iterator<ContentType>> mpIterator;

protected:
	ISearch(){}
	virtual ~ISearch(){}
};

typedef ISearch<FileInfo> FileSearch;

