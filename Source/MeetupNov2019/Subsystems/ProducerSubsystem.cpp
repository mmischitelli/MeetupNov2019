#include "ProducerSubsystem.h"
#include "PrinterSubsystem.h"
#include "Async/Async.h"
#include "Engine/Engine.h"
#include "MeetupNov2019/Common/Constants.h"

void UProducerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Tells Unreal that this subsystem depends on UPrinterSubsystem
	Collection.InitializeDependency(UPrinterSubsystem::StaticClass());

	m_Printer = GetGameInstance()->GetSubsystem<UPrinterSubsystem>();
	
	m_Producers.Reserve(m_Limits.Max);
	m_Producers.SetNum(m_Limits.Min);
	UStd::TGenerate_N(m_Producers.CreateIterator(), m_Limits.Min, [](){
		return MakeUnique<ProducerThread>();
	});
}

void UProducerSubsystem::Deinitialize()
{
	m_PendingKill = true;
	UStd::TForEach_N(m_Producers.CreateConstIterator(), m_Producers.Num(), [](const ProducerThreadPtr& thread) {
		thread->KillAndWait();
	});
}

bool UProducerSubsystem::AddNewProducer()
{
	if (m_Producers.Num() == m_Limits.Max) {
		return false;
	}

	m_Producers.Emplace(MakeUnique<ProducerThread>());

	return true;
}

bool UProducerSubsystem::RemoveProducer()
{
	if (m_Producers.Num() == m_Limits.Min) {
		return false;
	}

	m_Producers.Pop();
	
	return true;
}

void UProducerSubsystem::GetRandomValueAsync()
{
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&]()
	{
		TOptional<float> randValue;
		while (!randValue.IsSet())
		{
			if (m_PendingKill)
				return;
			
			randValue = TryGetRandomValue();
			if (!randValue.IsSet())
			{
				if (m_Printer) {
					m_Printer->PrintString(PKC::ProducerStr3, "Couldn't find any value, sleeping for a while...");
				}
				FPlatformProcess::Sleep(0.2);
			}
		}

		m_RandomValueReady.Broadcast(randValue.GetValue());
	});
}

TOptional<float> UProducerSubsystem::TryGetRandomValue() const
{
	TArray<int> AvailableProducersIndices;
	for(int i = 0; i < m_Producers.Num(); ++i)
	{
		if (m_Producers[i]->GetNumAvailable() > 0)
			AvailableProducersIndices.Add(i);
	}
	
	if (AvailableProducersIndices.Num() == 0)
		return {};

	const auto Index = m_Random.RandRange(0, AvailableProducersIndices.Num()-1);
	return m_Producers[AvailableProducersIndices[Index]]->GetNextRandomValue();
}

void UProducerSubsystem::PrintStats() const
{
	if (!m_Printer)
		return;
	
	m_Printer->PrintString(PKC::ProducerStr1, FString::Printf(TEXT("Producers: %d [%d, %d]"), m_Producers.Num(), m_Limits.Min, m_Limits.Max));

	const auto kNumValues = UStd::TAccumulate_N(m_Producers.CreateConstIterator(), m_Producers.Num(), 0, [](int curr, const ProducerThreadPtr& thread) {
		return curr + thread->GetNumAvailable();
	});

	m_Printer->PrintString(PKC::ProducerStr2, FString::Printf(TEXT("Available numbers: %d"), kNumValues));
}

FProducersStats UProducerSubsystem::GetStats() const
{
	FProducersStats stats;
	stats.ProducersInfo.SetNum(m_Producers.Num());
	UStd::TTransform_N(m_Producers.CreateConstIterator(), m_Producers.Num(), stats.ProducersInfo.CreateIterator(), [i = 0](const ProducerThreadPtr& producer) mutable  {
		return FProducerInfo{ "Producer#" + FString::FromInt(++i), producer->GetNumAvailable() };
	});
	return stats;
}
