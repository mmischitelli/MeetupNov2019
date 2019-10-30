#pragma once

#include "GameFramework/PlayerController.h"
#include "CurrentPlayerController.generated.h"


UCLASS()
class MEETUPNOV2019_API ACurrentPlayerController : public APlayerController
{
	GENERATED_BODY()

	class UPrinterSubsystem* m_Printer;
	class UProducerSubsystem* m_Producer;
	class AProducerViewer* m_ProducerViewer;
	
	bool m_WaitForAsyncTask;
	double m_Sum;

protected:
	ACurrentPlayerController();
	void InitPlayerState() override;
	void SetupInputComponent() override;
	void Tick(float DeltaSeconds) override;
		
private:
	void _SpawnNewProducer();
	void _KillProducer();

	UFUNCTION()
	void _GotRandomValue(float value);
};