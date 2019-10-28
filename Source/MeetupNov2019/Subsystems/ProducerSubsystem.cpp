#include "ProducerSubsystem.h"
#include "Engine/Engine.h"
#include "PrinterSubsystem.h"
#include "Common/Constants.h"
#include "Async.h"

void UProducerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// Tells Unreal that this subsystem depends on UPrinterSubsystem
	Collection.InitializeDependency(UPrinterSubsystem::StaticClass());

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
				GetGameInstance()->GetSubsystem<UPrinterSubsystem>()->PrintString(PKC::ProducerStr3, "Couldn't find any value, sleeping for a while...");
				FPlatformProcess::Sleep(0.2);
			}
		}

		m_RandomValueReady.Broadcast(randValue.GetValue());
	});
}

TOptional<float> UProducerSubsystem::TryGetRandomValue() const
{
	const auto kProducer = m_Producers.FindByPredicate([](const ProducerThreadPtr& producer) {
		return producer->GetNumAvailable() > 0;
	});

	if (kProducer != nullptr) {
		return (*kProducer)->GetNextRandomValue();
	}

	return {};
}

void UProducerSubsystem::PrintStats() const
{
	const auto kPrinter = GetGameInstance()->GetSubsystem<UPrinterSubsystem>();
	kPrinter->PrintString(PKC::ProducerStr1, FString::Printf(TEXT("Producers: %d [%d, %d]"), m_Producers.Num(), m_Limits.Min, m_Limits.Max));

	const auto kNumValues = UStd::TAccumulate_N(m_Producers.CreateConstIterator(), m_Producers.Num(), 0, [](int curr, const ProducerThreadPtr& thread) {
		return curr + thread->GetNumAvailable();
	});

	kPrinter->PrintString(PKC::ProducerStr2, FString::Printf(TEXT("Available numbers: %d"), kNumValues));
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
