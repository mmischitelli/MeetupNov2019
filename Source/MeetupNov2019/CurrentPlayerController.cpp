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
	
	m_Printer = GetGameInstance()->GetSubsystem<UPrinterSubsystem>();
	m_Producer = GetGameInstance()->GetSubsystem<UProducerSubsystem>();
	m_Producer->OnRandomValueReady().AddDynamic(this, &ACurrentPlayerController::_GotRandomValue);

	m_ProducerViewer = GetWorld()->SpawnActor<AProducerViewer>();
	m_ProducerViewer->SetActorLocationAndRotation(FVector{ 200, 0, 100 }, FRotator(0, 90, 0));
}

void ACurrentPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("SpawnNewProducer", EInputEvent::IE_Pressed, this, &ACurrentPlayerController::_SpawnNewProducer);
	InputComponent->BindAction("KillProducer", EInputEvent::IE_Pressed, this, &ACurrentPlayerController::_KillProducer);
}
void ACurrentPlayerController::Tick(float DeltaSeconds)
{
	if (m_WaitForAsyncTask)
		return;
	
	const auto kValue = m_Producer->TryGetRandomValue();
	if(kValue.IsSet()) {
		m_Sum += kValue.GetValue();
		return;
	}

	m_Printer->PrintString(PKC::PlayerCtrlStr1, FString::Printf(TEXT("Sum thus far: %.2f"), m_Sum));

	m_Sum = 0;
	m_WaitForAsyncTask = true;
	
	m_Producer->GetRandomValueAsync();
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

void ACurrentPlayerController::_GotRandomValue(float value)
{
	m_Printer->PrintString(PKC::PlayerCtrlStr2, FString::Printf(TEXT("Received new value: %0.2f"), value));
	AsyncTask(ENamedThreads::GameThread, [&]() { m_WaitForAsyncTask = false; });
}
