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
	/** The actual thread this FRunnable runs on */
	FRunnableThread* m_Thread;
	/** Mutex to synchronize access to m_RandomData */
	FCriticalSection m_Mutex;
	/** Semaphore used to wake up the thread from sleep */
	FEvent* m_Semaphore;

	/** Thread safe flag used to kill this thread gracefully */
	FThreadSafeBool m_PendingKill;
	/** Thread safe flag used to put this thread to sleep */
	FThreadSafeBool m_Pause;
	/** Struct that holds initialization data */
	struct InitDataStruct
	{
		int NumElements;
		int ChunkSize;
	} m_InitData;

	/** Unreal's PRNG facility*/
	FRandomStream m_RNG;
	/** Stock of generated numbers */
	TArray<float> m_RandomData;
	
public:
	ProducerThread(int numElems = 16, int chunks = 2);
	~ProducerThread();

	//~ Begin FRunnable Interface.
	bool Init() override;
	uint32 Run() override;
	void Stop() override;
	//~ End FRunnable Interface.

	/** Wait for this thread to actually die */
	void KillAndWait();
	/** Puts this producer to sleep */
	void Pause();
	/** Resumes producing random numbers */
	void Continue();

	/**
	 * Returns a float value, if available.
	 * @return An optional which might contain a new random float if this thread had enough time to generate it.
	 */
	TOptional<float> GetNextRandomValue();
	/**
	 * Use to know how many random values there are inside this producer
	 * @return The number of available random values
	 */
	int GetNumAvailable() const { return m_RandomData.Num(); }

private:
	TArray<float> _FillChunk(int chunkSize) const;
};
