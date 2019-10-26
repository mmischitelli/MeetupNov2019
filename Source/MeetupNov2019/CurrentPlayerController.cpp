#include "CurrentPlayerController.h"
#include "Engine/GameInstance.h"
#include "Subsystems/PrinterSubsystem.h"

void ACurrentPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	
	m_Printer = GetGameInstance()->GetSubsystem<UPrinterSubsystem>();
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
}

void ACurrentPlayerController::_KillProducer()
{
	m_Printer->PrintString("Killing a producer...");
}
