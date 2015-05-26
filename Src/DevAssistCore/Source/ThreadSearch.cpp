#include "stdafx.h"
#include "ThreadSearch.h"


		ThreadSearch::ThreadSearch( std::shared_ptr<Iterator<FileInfo>> pIterator, IResultContainer<FileInfo>* pContainer) :
			ISearch( pIterator, pContainer ), mlStop(0)
		{
			_tprintf( L"ThreadSearch \n" );
			Init();
			_tprintf( L"ThreadSearch after Init\n" );
			WaitForSingleObject( mhStartAckEvent , INFINITE );
			_tprintf( L"ThreadSearch after wait\n" );
		}


		ThreadSearch::~ThreadSearch()
		{
			_tprintf( L"~ThreadSearch \n" );
			KillThread();

			CloseHandle( mhSearchThread);
			CloseHandle( mhStartEvent);
			CloseHandle( mhExitEvent);
			CloseHandle(mhIdleEvent);
			CloseHandle(mhStartAckEvent);
		}

		bool ThreadSearch::StartSearch(std::shared_ptr<Iterator<FileInfo>> pIterator, ISearchCrieteria<FileInfo>* pCrieteria)
		{
			// make sure its not runnning
			Stop();
			_tprintf( L"StartSearch after stop\n" );
			mpCrieteria = pCrieteria;
			mpIterator = pIterator;
			InterlockedExchange( &mlStop, 0 );

			SetEvent( mhStartEvent );
			_tprintf( L"StartSearch after setevent\n" );

			WaitForSingleObject( mhStartAckEvent , INFINITE );
			_tprintf( L"StartSearch after ACK\n" );
			return true;
		}

		bool ThreadSearch::Stop()
		{
			InterlockedExchange( &mlStop, 1 );

			MSG msg;
			while (true) {
				switch (MsgWaitForMultipleObjects(1, &mhIdleEvent,
					FALSE, INFINITE, QS_ALLINPUT)) {
				case WAIT_OBJECT_0:
					return true; // event has been signalled
					break;
				case WAIT_OBJECT_0+1:
					// we have a message - peek and dispatch it
					while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
						// TODO:  must handle WM_QUIT; see Raymond's blog for details
						TranslateMessage(&msg);
						DispatchMessage(&msg);

						//if( msg.message == WM_QUIT )
						//{
						//	return false;
						//}
					}
					break;
				default:
					return FALSE; // unexpected failure
				}
			}


			// WaitForSingleObject( mhIdleEvent , INFINITE );
			_tprintf( L"Stop after wait \n" );
			return true;
		}

		bool ThreadSearch::WaitForFinish()
		{
			WaitForSingleObject( mhIdleEvent , INFINITE );
			return true;
		}



		void ThreadSearch::Init()
		{
			mhExitEvent = CreateEvent( 0, TRUE, FALSE, 0 );
			mhStartEvent = CreateEvent( 0, FALSE, FALSE, 0 );
			mhIdleEvent = CreateEvent(0, TRUE, TRUE, 0);
			mhStartAckEvent = CreateEvent( 0, FALSE,FALSE,0);
			mhSearchThread = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this,0,0 );
		}

		 UINT ThreadSearch::ThreadProc(LPVOID lpThis)
		{
			_tprintf( L"ThreadProc \n");
			ThreadSearch* pThis = (ThreadSearch*)(lpThis);
			pThis->SearchImpl();
			return 0;
		}

		void ThreadSearch::SearchImpl()
		{
			_tprintf( L"SearchImpl \n");
			SetEvent( mhStartAckEvent );
			_tprintf( L"SearchImpl set idle\n");

			HANDLE hArray[] = {mhStartEvent,mhExitEvent};
			while( 1 )
			{
				DWORD dwWait = WaitForMultipleObjects( 2, hArray, FALSE, INFINITE );
				_tprintf( L"SearchImpl after waiit\n");
				if( WAIT_OBJECT_0 == dwWait )
				{
					ResetEvent( mhIdleEvent );
					_tprintf( L"SearchImpl after reset entering loop\n");

					SetEvent(mhStartAckEvent);
					_tprintf( L"SearchImpl start signaled\n");
					while(mpIterator->HasNext()) 
					{
						if(IsStop())
						{
							_tprintf( L"SearchImpl IsStop 1\n");
							break;
						}
						FileInfo* pContent = mpIterator->Next();
						if( mpCrieteria->Validate(pContent))
						{
							mpContainer->AddResult(pContent);
						}
					}
					mpContainer->NotifySearchEnd();
					_tprintf( L"SearchImpl after loop\n");
					SetEvent( mhIdleEvent );
					_tprintf( L"SearchImpl after set event\n");
				}
				else
				{
					_tprintf( L"SearchImpl wait other than 0\n");
					return;
				}
			}
		}

		void ThreadSearch::KillThread()
		{
			_tprintf( L"KillThread \n");
			Stop();
			SetEvent( mhExitEvent );
			_tprintf( L"KillThread after set event\n");
			// Wait for thread to exit
			WaitForSingleObject ( mhSearchThread, INFINITE);
			_tprintf( L"KillThread after wait\n");
		}

		bool ThreadSearch::IsStop()
		{
			_tprintf( L"IsStop %d \n", int(mlStop));
			long lVal = InterlockedCompareExchange( &mlStop, 1, 1 );
			return lVal > 0 ? true : false;
		}

