#include "CurrentPlayerController.h"
#include "Engine/GameInstance.h"
#include "Subsystems/PrinterSubsystem.h"
#include "Subsystems/ProducerSubsystem.h"
#include "TimerManager.h"
#include "Common/Constants.h"

void ACurrentPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	
	m_Printer = GetGameInstance()->GetSubsystem<UPrinterSubsystem>();
	m_Producer = GetGameInstance()->GetSubsystem<UProducerSubsystem>();

	// Prints stats every half a second
	GetWorldTimerManager().SetTimer(m_StatsTimerHandle, this, &ACurrentPlayerController::_PrintStats, 0.5f, true);
}

void ACurrentPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("SpawnNewProducer", EInputEvent::IE_Pressed, this, &ACurrentPlayerController::_SpawnNewProducer);
	InputComponent->BindAction("KillProducer", EInputEvent::IE_Pressed, this, &ACurrentPlayerController::_KillProducer);
}

void ACurrentPlayerController::_SpawnNewProducer()
{
	m_Printer->PrintString(PKC::PlayerCtrlStr1, "Spawning a new producer...");
	m_Producer->AddNewProducer();
}

void ACurrentPlayerController::_KillProducer()
{
	m_Printer->PrintString(PKC::PlayerCtrlStr2, "Killing a producer...");
	m_Producer->RemoveProducer();
}

void ACurrentPlayerController::_PrintStats() const
{
	m_Producer->PrintStats();
}
