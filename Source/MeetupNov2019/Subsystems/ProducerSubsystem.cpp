#include "ProducerSubsystem.h"
#include "Engine/Engine.h"
#include "PrinterSubsystem.h"
#include "Common/Constants.h"

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

void UProducerSubsystem::PrintStats() const
{
	const auto kPrinter = GetGameInstance()->GetSubsystem<UPrinterSubsystem>();
	kPrinter->PrintString(PKC::ProducerStr1, FString::Printf(TEXT("Producers: %d [%d, %d]"), m_Producers.Num(), m_Limits.Min, m_Limits.Max));

	const auto kNumValues = UStd::TAccumulate_N(m_Producers.CreateConstIterator(), m_Producers.Num(), 0, [](int curr, const ProducerThreadPtr& thread) {
		return curr + thread->GetNumAvailable();
	});

	kPrinter->PrintString(PKC::ProducerStr2, FString::Printf(TEXT("Available numbers: %d"), kNumValues));
}
