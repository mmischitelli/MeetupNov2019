#include "CurrentPlayerController.h"
#include "Engine/GameInstance.h"
#include "Subsystems/PrinterSubsystem.h"
#include "Subsystems/ProducerSubsystem.h"
#include "TimerManager.h"
#include "Common/Constants.h"
#include "Async.h"
#include "Engine/World.h"
#include "Actors/ProducerViewer.h"
#include "Actors/HorizontalLayoutComponent.h"

ACurrentPlayerController::ACurrentPlayerController()
	: m_WaitForAsyncTask(false)
	, m_Sum(0)
{ }

void ACurrentPlayerController::InitPlayerState()
{
	Super::InitPlayerState();

	auto gameInstance = GetGameInstance();
	if (gameInstance)
	{
		m_Printer = GetGameInstance()->GetSubsystem<UPrinterSubsystem>();
		m_Producer = GetGameInstance()->GetSubsystem<UProducerSubsystem>();
		m_Producer->OnRandomValueReady().AddDynamic(this, &ACurrentPlayerController::_GotRandomValue);
	}
	
	if (m_ProducerViewerClass)
	{
		m_ProducerViewer = GetWorld()->SpawnActor<AProducerViewer>(m_ProducerViewerClass);
		m_ProducerViewer->SetActorLocationAndRotation(FVector{ 200, 0, 0 }, FRotator(0, 90, 0));
	}

	GetWorldTimerManager().SetTimer(m_LongTickTimer, this, &ACurrentPlayerController::_ConsumeRandomValue, 0.15f, true);
}

void ACurrentPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SpawnNewProducer", EInputEvent::IE_Pressed, this, &ACurrentPlayerController::_SpawnNewProducer);
	InputComponent->BindAction("KillProducer", EInputEvent::IE_Pressed, this, &ACurrentPlayerController::_KillProducer);
}

void ACurrentPlayerController::_SpawnNewProducer()
{
	m_Printer->PrintString("Spawning a new producer...");
	m_Producer->AddNewProducer();
}

void ACurrentPlayerController::_KillProducer()
{
	m_Printer->PrintString("Killing a producer...");
	m_Producer->RemoveProducer();
}

void ACurrentPlayerController::_ConsumeRandomValue()
{
	if (m_WaitForAsyncTask)
		return;

	const auto kValue = m_Producer->TryGetRandomValue();
	if (kValue.IsSet()) {
		m_Sum += kValue.GetValue();
		return;
	}

	m_Printer->PrintString(PKC::PlayerCtrlStr1, FString::Printf(TEXT("There are no values available... waiting for a new one. Sum thus far: %.2f"), m_Sum));

	m_Sum = 0;
	m_WaitForAsyncTask = true;

	m_Producer->GetRandomValueAsync();
}

void ACurrentPlayerController::_GotRandomValue(float value)
{
	m_Printer->PrintString(PKC::PlayerCtrlStr2, TEXT("New value received, resuming the consumer..."));
	m_WaitForAsyncTask = false;
}
