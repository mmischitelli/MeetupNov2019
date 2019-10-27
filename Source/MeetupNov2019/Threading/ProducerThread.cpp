#include "ProducerThread.h"
#include "GenericPlatformProcess.h"
#include "RunnableThread.h"
#include "PlatformProcess.h"
#include "Event.h"
#include "RandomStream.h"
#include "Common\Utils.h"
#include <algorithm>

ProducerThread::ProducerThread(int numElems, int chunkSize)
	: m_PendingKill(false)
	, m_Pause(false)
	, m_InitData({ numElems, chunkSize })
{
	m_Semaphore = FGenericPlatformProcess::GetSynchEventFromPool(false);
	m_RandomData.Reserve(numElems);
	m_Thread = FRunnableThread::Create(this, TEXT("ProducerThread"), 0, TPri_BelowNormal);
}

ProducerThread::~ProducerThread()
{
	// Release the event and set it to null
	if (m_Semaphore)
	{
		FGenericPlatformProcess::ReturnSynchEventToPool(m_Semaphore);
		m_Semaphore = nullptr;
	}

	// Kill the thread and set it to null
	if (m_Thread)
	{
		delete m_Thread;
		m_Thread = nullptr;
	}
}

bool ProducerThread::Init()
{
	m_RandomData.Empty();
	return true;
}

uint32 ProducerThread::Run()
{
	int filled = 0;

	while (!m_PendingKill)
	{
		if (m_Pause)
		{
			m_Semaphore->Wait();
			if (m_PendingKill)
				return 0;
		}
		else
		{
			const auto kChunk = _FillChunk(std::min(m_InitData.ChunkSize, m_InitData.NumElements - filled));
			filled += kChunk.Num();

			m_Mutex.Lock();
			m_RandomData.Append(kChunk);
			m_Mutex.Unlock();

			// Generate a chunk of random values every 100ms
			FPlatformProcess::Sleep(0.1);
		}
	}

	return 0;
}

void ProducerThread::Stop()
{
	m_PendingKill = true;
	m_Pause = false;

	if (m_Semaphore) {
		m_Semaphore->Trigger();	//Wake up the thread!
	}
}

void ProducerThread::KillAndWait()
{
	Stop();

	if (m_Thread) {
		m_Thread->WaitForCompletion();
	}
}

void ProducerThread::Pause()
{
	m_Pause = true;
}

void ProducerThread::Continue()
{
	m_Pause = false;
	
	if (m_Semaphore) {
		m_Semaphore->Trigger();	//Wake up the thread!
	}
}

TOptional<float> ProducerThread::GetNextRandomValue()
{
	m_Mutex.Lock();
	
	if (m_RandomData.Num() == 0)
	{
		m_Mutex.Unlock();
		return {};
	}

	const auto kNextRandom = m_RandomData.Pop();
	m_Mutex.Unlock();
	
	return kNextRandom;
}

TArray<float> ProducerThread::_FillChunk(int chunkSize) const
{
	TArray<float> currentChunk;
	currentChunk.SetNumUninitialized(chunkSize);
	UStd::TGenerate_N(currentChunk.CreateIterator(), chunkSize, [&]() {
		return m_RNG.FRand();
	});
	return currentChunk;
}
