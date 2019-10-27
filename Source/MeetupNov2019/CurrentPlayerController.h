#pragma once

#include "GameFramework/PlayerController.h"
#include "CurrentPlayerController.generated.h"

class UPrinterSubsystem;
class UProducerSubsystem;

UCLASS()
class MEETUPNOV2019_API ACurrentPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPrinterSubsystem* m_Printer;
	UProducerSubsystem* m_Producer;
	FTimerHandle m_StatsTimerHandle;

protected:
	void InitPlayerState() override;
	void SetupInputComponent() override;

private:
	void _SpawnNewProducer();
	void _KillProducer();
	void _PrintStats() const;
};