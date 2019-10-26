#pragma once

#include "Runnable.h"
#include "WindowsCriticalSection.h"
#include "ThreadSafeBool.h"
#include "Array.h"
#include "RandomStream.h"

class FWindowsCriticalSection;
class FRunnableThread;

/**
 * @class ProducerThread
 * @brief Fills an array of random float values
 */
class MEETUPNOV2019_API ProducerThread : public FRunnable
{
	FRunnableThread* m_Thread;

	FCriticalSection m_Mutex;
	FEvent* m_Semaphore;
	
	FThreadSafeBool m_PendingKill;
	FThreadSafeBool m_Pause;

	struct InitDataStruct
	{
		int NumElements;
		int ChunkSize;
	} m_InitData;

	FRandomStream m_RNG;
	TArray<float> m_RandomData;
	
public:
	ProducerThread(int numElems = 30, int chunks = 5);
	~ProducerThread();

	//~ Begin FRunnable Interface.
	bool Init() override;
	uint32 Run() override;
	void Stop() override;
	//~ End FRunnable Interface.

	void KillAndWait();
	void Pause();
	void Continue();

	/**
	 * Returns a float value, if available.
	 * @return An optional which might contain the a new random float if this thread had enough time to generate some.
	 */
	TOptional<float> GetNextRandomValue();

private:
	TArray<float> _FillChunk(int chunkSize) const;
};
