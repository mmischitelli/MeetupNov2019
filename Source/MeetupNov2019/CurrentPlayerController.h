#pragma once

#include "GameFramework/PlayerController.h"
#include "CurrentPlayerController.generated.h"

class UPrinterSubsystem;

UCLASS()
class MEETUPNOV2019_API ACurrentPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPrinterSubsystem* m_Printer;

protected:
	void InitPlayerState() override;
	void SetupInputComponent() override;

private:
	void _SpawnNewProducer();
	void _KillProducer();
};