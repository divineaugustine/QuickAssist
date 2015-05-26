#pragma once
#include <Core\DataStructs.h>

	class ThreadSearch : public FileSearch
	{
	public:
		ThreadSearch(std::shared_ptr<Iterator<FileInfo>> pIterator, IResultContainer<FileInfo>* pContainer);
		~ThreadSearch();
		bool StartSearch(std::shared_ptr<Iterator<FileInfo>> pIterator, ISearchCrieteria<FileInfo>* pCrieteria);
		bool Stop();
		bool WaitForFinish();

	private:

		void Init();
		static UINT ThreadProc(LPVOID lpThis);
		void SearchImpl();
		void KillThread();
		bool IsStop();

	private:

		HANDLE mhSearchThread;
		HANDLE mhStartEvent;
		HANDLE mhExitEvent;
		HANDLE mhIdleEvent;		
		HANDLE mhStartAckEvent;
		volatile long mlStop;
	};